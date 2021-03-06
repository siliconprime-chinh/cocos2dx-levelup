/*
 Copyright (C) 2012-2014 Soomla Inc.

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


#include "CCGatesListOr.h"

namespace soomla {

    USING_NS_CC;

    CCGatesListOr *CCGatesListOr::create(cocos2d::__String *id, cocos2d::__Array *gates) {
        CCGatesListOr *ret = new CCGatesListOr();
        if (ret->init(id, gates)) {
            ret->autorelease();
        }
        else {
            CC_SAFE_DELETE(ret);
        }

        return ret;
    }

    CCGatesListOr *CCGatesListOr::create(cocos2d::__String *id, CCGate *singleGate) {
        CCGatesListOr *ret = new CCGatesListOr();
        if (ret->init(id, singleGate)) {
            ret->autorelease();
        }
        else {
            CC_SAFE_DELETE(ret);
        }

        return ret;
    }

    bool CCGatesListOr::canOpenInner() {
        Ref *ref;
        CCGate *gate;
        CCARRAY_FOREACH(getGates(), ref) {
                gate = dynamic_cast<CCGate *>(ref);
                CC_ASSERT(gate);
                if (gate->isOpen()) {
                    return true;
                }
            }
        return false;
    }

    const char *CCGatesListOr::getType() const {
        return CCLevelUpConsts::JSON_JSON_TYPE_GATES_LIST_OR;
    }
}
