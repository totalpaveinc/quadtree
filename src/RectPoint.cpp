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

#include <tp/qt/RectPoint.h>

namespace TP { namespace qt {
    RectPoint::RectPoint(geom::Extent<double> extent) {
        $extent = extent;
    }

    RectPoint::RectPoint(geom::Extent<double> extent, const void* data): QuadPoint(data) {
        $extent = extent;
    }

    const geom::Extent<double> RectPoint::getExtent(void) const {
        return $extent;
    }

    const bool RectPoint::isInBounds(const geom::Extent<double>& extent) const {
        return $extent.isInBounds(extent);
    }
}}
