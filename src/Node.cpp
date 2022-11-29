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

    Node::Node(uint32_t bucketSize, const Extent<double>& extent) {
        $extent = extent;
        $bucketSize = bucketSize;

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

    void Node::query(const Extent<double>& extent, std::vector<const void*>& dataList, std::unordered_map<long, bool>& dataManifest) {
        if ($children.size() > 0 && $extent.isInBounds(extent)) {
            for (std::size_t i = 0; i < $children.size(); i++) {
                const QuadPoint* point = $children[i];
                const void* data = point->getData();
                long ptr = (long)data;
                if (dataManifest.find(ptr) == dataManifest.end()) {
                    dataManifest.insert(std::pair<long, bool>(ptr, true));
                    dataList.push_back(data);
                }
            }
        }

        if ($nw != nullptr) {
            $nw->query(extent, dataList, dataManifest);
            $ne->query(extent, dataList, dataManifest);
            $sw->query(extent, dataList, dataManifest);
            $se->query(extent, dataList, dataManifest);
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

        // If we made it here, then we are at a leaf node.
        $children.push_back(point);

        if ($children.size() >= $bucketSize) {
            subdivide();
        }
    }

    void Node::subdivide(void) {
        if ($nw != nullptr) {
            // This node has already been subdivided.
            return;
        }

        Extent<double> quadExtent = Extent<double>::quad($extent);
        double tx, ty;
        quadExtent.getRange(tx, ty);

        $nw = new Node($bucketSize, quadExtent);

        quadExtent.translate(tx, 0.0);
        $ne = new Node($bucketSize, quadExtent);

        quadExtent.translate(0.0, ty);
        $se = new Node($bucketSize, quadExtent);

        quadExtent.translate(-tx, 0.0);
        $sw = new Node($bucketSize, quadExtent);

        const Extent<double>& nwExtent = $nw->getExtent();
        const Extent<double>& neExtent = $ne->getExtent();
        const Extent<double>& swExtent = $sw->getExtent();
        const Extent<double>& seExtent = $se->getExtent();

        std::vector<const QuadPoint*>::iterator iterator = $children.begin();

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
    }
}}
