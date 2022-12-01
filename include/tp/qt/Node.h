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

#pragma once

#include <tp/geom/Extent.h>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <tp/qt/QuadPoint.h>

namespace TP { namespace qt {
    class Node {
        public:
            Node(const geom::Extent<double>& extent);
            virtual ~Node();

            const geom::Extent<double>& getExtent(void) const;
            void subdivide(void);

            void insert(std::size_t index, const QuadPoint* point);

            void query(const geom::Extent<double>& extent, std::vector<const void*>& data, std::unordered_map<long, bool>& dataManifest);

        private:
            struct PointChild {
                std::size_t index;
                Node::PointChild* next;
            };
            geom::Extent<double> $extent;
            Node* $nw;
            Node* $ne;
            Node* $sw;
            Node* $se;
            Node::PointChild* $child;
            std::size_t $childCount;
    };
}}
