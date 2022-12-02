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

#include <tp/qt/QuadTree.h>

namespace TP { namespace qt {
    using namespace TP::geom;

    // Note maxDepth has a default value declared in the headers.
    QuadTree::QuadTree(uint32_t bucketSize, const Extent<double>& extent, uint8_t maxDepth) {
        $root = new Node(bucketSize, extent, maxDepth);
    }

    QuadTree::~QuadTree() {
        delete $root;

        for (std::size_t i = 0; i < $data.size(); i++) {
            delete $data[i];
        }
    }

    void QuadTree::insert(double x, double y, const void* data) {
        XYPoint* point = new XYPoint(x, y, data);
        insert(point, data);
    }
    void QuadTree::insert(XYPoint* point, const void* data) {
        $data.push_back(point);
        $root->insert(point);
    }

    void QuadTree::insert(double x1, double y1, double x2, double y2, const void* data) {
        const geom::Extent<double>* extent = new geom::Extent(x1, y1, x2, y2);
        insert(*extent, data);
    }
    void QuadTree::insert(const geom::Extent<double>& extent, const void* data) {
        RectPoint* point = new RectPoint(extent, data);
        insert(point, data);
    }
    void QuadTree::insert(RectPoint* point, const void* data) {
        $data.push_back(point);
        $root->insert(point);
    }

    void QuadTree::query(const geom::Extent<double>& extent, std::vector<const void*>& outData) {
        outData.clear();
        // This is used to keep track of data to ensure only unique data is inserted into the out array.
        // as there could be many points to one user dataset.
        std::unordered_map<long, bool> manifest;
        $root->query(extent, outData, manifest);
    }
}}
