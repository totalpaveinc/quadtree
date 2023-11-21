/*
    Copyright 2022 Total Pave Inc

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <tp/qt/Node.h>
#include <stdexcept>

namespace TP { namespace qt {
    using namespace TP::geom;

    Node::Node(uint32_t bucketSize, const Extent<double>& extent, uint8_t depth) {
        $extent = extent;
        $bucketSize = bucketSize;
        $depth = depth;

        // These only exists once this node has been subdivided
        $nw = nullptr;
        $ne = nullptr;
        $sw = nullptr;
        $se = nullptr;
    }

    Node::~Node() {
        if ($nw != nullptr) {
            delete $nw;
            delete $ne;
            delete $sw;
            delete $se;
        }
    }

    const Extent<double>& Node::getExtent(void) const {
        return $extent;
    }

    void Node::query(const Extent<double>& extent, std::vector<const void*>& dataList, std::unordered_map<long, bool>& dataManifest, std::function<bool(const Extent<double>&, const QuadPoint*)>* filter) {
        // isInBounds is the first pass filter.
        // It checks to see if the requested extent is at least partially within the bounds of the quad extent.
        // The first pass filter is a very broad and inaccurate filter, it often includes many false possitives.
        // The false positives happen because quad extent is bigger than the requested extent, it ends up selecting
        // a lot of extra data in the general surrounding area of the requested extent.
        // The quad extent is often bigger than the requested extent because there is not enough data for the quad to split into smaller quads.
        if ($extent.isInBounds(extent)) {
            for (std::size_t i = 0; i < $children.size(); i++) {
                const QuadPoint* point = $children[i];
                const void* data = point->getData();
                long ptr = (long)data;
                if (
                    dataManifest.find(ptr) == dataManifest.end() &&
                    // Second pass filtering
                    // Test each QuadPoint against the requested extent.
                    // The second pass filter has medium accuracy.
                    // Most false positives are caught but long diagonal lines still have many false positives.
                    point->isInBounds(extent)
                ) {
                    if (
                        // If filter is defined...
                        filter != nullptr &&
                        !((*filter)(extent, point))
                    ) { // If point fails filter then continue to next point.
                        continue;
                    }
                    dataManifest.insert(std::pair<long, bool>(ptr, true));
                    dataList.push_back(data);
                }
            }

            if ($nw != nullptr) {
                $nw->query(extent, dataList, dataManifest, filter);
                $ne->query(extent, dataList, dataManifest, filter);
                $sw->query(extent, dataList, dataManifest, filter);
                $se->query(extent, dataList, dataManifest, filter);
            }
        }
    }

    void Node::insert(const QuadPoint* point) {
        if ($nw != nullptr) {
            // This node has quadrants, so it should be passed down.

            const Extent<double>& nwExtent = $nw->getExtent();
            const Extent<double>& neExtent = $ne->getExtent();
            const Extent<double>& swExtent = $sw->getExtent();
            const Extent<double>& seExtent = $se->getExtent();

            if (point->isInBounds(nwExtent)) {
                $nw->insert(point);
            }

            if (point->isInBounds(neExtent)) {
                $ne->insert(point);
            }

            if (point->isInBounds(swExtent)) {
                $sw->insert(point);
            }

            if (point->isInBounds(seExtent)) {
                $se->insert(point);
            }

            return;
        }

        if ($children.size() == 0) {
            $children.reserve($bucketSize);
        }
        // If we made it here, then we are at a leaf node.
        $children.push_back(point);

        if ($children.size() >= $bucketSize && $depth != 1) {
            subdivide();
        }
    }

    void Node::subdivide(void) {
        if ($nw != nullptr || $depth == 1) {
            // This node has already been subdivided.
            return;
        }

        Extent<double> quadExtent = Extent<double>::quad($extent);
        double tx, ty;
        uint8_t childDepth = $depth - 1;
        
        quadExtent.getRange(tx, ty);
        $nw = new Node($bucketSize, quadExtent, childDepth);

        quadExtent.translate(tx, 0.0);
        $ne = new Node($bucketSize, quadExtent, childDepth);

        quadExtent.translate(0.0, ty);
        $se = new Node($bucketSize, quadExtent, childDepth);

        quadExtent.translate(-tx, 0.0);
        $sw = new Node($bucketSize, quadExtent, childDepth);

        const Extent<double>& nwExtent = $nw->getExtent();
        const Extent<double>& neExtent = $ne->getExtent();
        const Extent<double>& swExtent = $sw->getExtent();
        const Extent<double>& seExtent = $se->getExtent();

        while ($children.size() > 0) {
            const QuadPoint* point = $children.back();
            $children.pop_back();

            if (point->isInBounds(nwExtent)) {
                $nw->insert(point);
            }

            if (point->isInBounds(neExtent)) {
                $ne->insert(point);
            }

            if (point->isInBounds(swExtent)) {
                $sw->insert(point);
            }

            if (point->isInBounds(seExtent)) {
                $se->insert(point);
            }
        }

        $children.shrink_to_fit();
    }
}}
