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

#include <tp/qt/Point.h>

namespace TP { namespace qt {
    Point::Point(void) {
        $x = 0.0;
        $y = 0.0;
        $data = nullptr;
    }

    Point::Point(double x, double y) {
        $x = x;
        $y = y;
        $data = nullptr;
    }

    Point::Point(double x, double y, const void* data) {
        $x = x;
        $y = y;
        $data = data;
    }

    Point::~Point() {
        $x = 0.0;
        $y = 0.0;
        $data = nullptr;
    }

    double Point::getX(void) const {
        return $x;
    }

    double Point::getY(void) const {
        return $y;
    }

    const void* Point::getData(void) const {
        return $data;
    }
}}
