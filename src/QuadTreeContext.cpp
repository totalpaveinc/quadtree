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

#include <tp/qt/QuadTreeContext.h>

namespace TP { namespace qt {
    using namespace TP::geom;

    QuadTreeContext* QuadTreeContext::$instance = nullptr;

    QuadTreeContext::QuadTreeContext() {}

    uint32_t QuadTreeContext::getBucketSize(void) {
        return $size;
    }
    QuadPoint* QuadTreeContext::getPoint(std::size_t index) {
        return $data->at(index);
    }

    void QuadTreeContext::setBucketSize(uint32_t size) {
        $size = size;
    }
    void QuadTreeContext::setData(std::vector<QuadPoint*>* data) {
        $data = data;
    }
}}
