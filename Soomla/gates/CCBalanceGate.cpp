//
// Created by Shubin Fedor on 20/08/14.
// Copyright (c) 2014 SOOMLA. All rights reserved.
//

#include "CCBalanceGate.h"
#include "CCStoreInventory.h"
#include "CCSoomlaUtils.h"
#include "CCStoreEventDispatcher.h"

namespace soomla {

    soomla::CCBalanceGate *soomla::CCBalanceGate::create(cocos2d::__String *id, cocos2d::__String *associatedItemId, cocos2d::__Integer *desiredBalance) {
        CCBalanceGate *ret = new CCBalanceGate();
        if (ret->init(id, associatedItemId, desiredBalance)) {
            ret->autorelease();
        }
        else {
            CC_SAFE_DELETE(ret);
        }

        return ret;
    }

    bool soomla::CCBalanceGate::init(cocos2d::__String *id, cocos2d::__String *associatedItemId, cocos2d::__Integer *desiredBalance) {
        bool result = CCGate::init(id, NULL);
        if (result) {
            setAssociatedItemId(associatedItemId);
            setDesiredBalance(desiredBalance);
            return true;
        }
        return result;
    }

    bool soomla::CCBalanceGate::initWithDictionary(cocos2d::__Dictionary *dict) {
        bool result = CCGate::initWithDictionary(dict);
        if (result) {
            fillAssociatedItemIdFromDict(dict);
            fillDesiredBalanceFromDict(dict);
            return true;
        }
        return result;
    }

    const char *soomla::CCBalanceGate::getType() const {
        return CCLevelUpConsts::JSON_JSON_TYPE_BALANCE_GATE;
    }

    cocos2d::__Dictionary *soomla::CCBalanceGate::toDictionary() {
        cocos2d::__Dictionary *dict = CCGate::toDictionary();

        putAssociatedItemIdToDict(dict);
        putDesiredBalanceToDict(dict);

        return dict;
    }

    soomla::CCBalanceGate::~CCBalanceGate() {
        CC_SAFE_RELEASE(mAssociatedItemId);
        CC_SAFE_RELEASE(mDesiredBalance);
        CC_SAFE_RELEASE(mEventHandler);
    }

    bool CCBalanceGate::canOpenInner() {
        CCError *error = NULL;
        int balance = CCStoreInventory::sharedStoreInventory()->getItemBalance(mAssociatedItemId->getCString(), &error);
        if (error) {
            CCSoomlaUtils::logError(TAG, cocos2d::__String::createWithFormat(
                    "(canOpenInner) Couldn't find itemId. itemId: %s", mAssociatedItemId->getCString())->getCString());
            CCSoomlaUtils::logError(TAG, error->getInfo());
            return false;
        }
        return (balance >= mDesiredBalance->getValue());
    }


    bool CCBalanceGate::openInner() {
        if (canOpen()) {
            // There's nothing to do here... If the DesiredBalance was reached then the gate is just open.
            forceOpen(true);
            return true;
        }

        return false;
    }


    void CCBalanceGate::registerEvents() {
        if (!isOpen()) {
            setEventHandler(CCBalanceGateEventHandler::create(this));
            CCStoreEventDispatcher::getInstance()->addEventHandler(getEventHandler());
        }
    }

    void CCBalanceGate::unregisterEvents() {
        CCStoreEventHandler *eventHandler = getEventHandler();
        if (eventHandler) {
            CCStoreEventDispatcher::getInstance()->removeEventHandler(eventHandler);
            setEventHandler(NULL);
        }
    }

    void CCBalanceGate::checkItemIdBalance(cocos2d::__String *itemId, int balance) {
        if (mAssociatedItemId->compare(itemId->getCString()) == 0 && balance >= mDesiredBalance->getValue()) {
            forceOpen(true);
        }
    }

    CCBalanceGateEventHandler *CCBalanceGateEventHandler::create(CCBalanceGate *balanceGate) {
        CCBalanceGateEventHandler *ret = new CCBalanceGateEventHandler();
        ret->autorelease();

        ret->mBalanceGate = balanceGate;

        return ret;
    }

    void CCBalanceGateEventHandler::onCurrencyBalanceChanged(CCVirtualCurrency *virtualCurrency, int balance, int amountAdded) {
        mBalanceGate->checkItemIdBalance(virtualCurrency->getItemId(), balance);
    }

    void CCBalanceGateEventHandler::onGoodBalanceChanged(CCVirtualGood *virtualGood, int balance, int amountAdded) {
        mBalanceGate->checkItemIdBalance(virtualGood->getItemId(), balance);
    }

}