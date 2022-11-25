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

#include <tp/qt/DepthQuadTree.h>

namespace TP { namespace qt {
    using namespace TP::geom;

    DepthQuadTree::DepthQuadTree(uint32_t depth, const Extent<double>& extent) {
        $root = new DepthNode(depth, extent);
    }

    DepthQuadTree::~DepthQuadTree() {
        delete $root;

        for (std::size_t i = 0; i < $data.size(); i++) {
            delete $data[i];
        }
    }

    void DepthQuadTree::insert(double x, double y, const void* data) {
        Point* point = new Point(x, y, data);
        $data.push_back(point);
        $root->insert(point);
    }

    void DepthQuadTree::query(const Extent<double>& extent, std::vector<const void*>& outData) {
        outData.clear();
        // This is used to keep track of data to ensure only unique data is inserted into the out array.
        // as there could be many points to one user dataset.
        std::unordered_map<long, bool> manifest;
        $root->query(extent, outData, manifest);
    }
}}
