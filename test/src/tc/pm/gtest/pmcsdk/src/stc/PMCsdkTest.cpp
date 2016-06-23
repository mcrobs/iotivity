/******************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      LICENSE-2.0" target="_blank">http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 ******************************************************************/

#include "PMCsdkHelper.h"

class PMCsdkTest_stc: public ::testing::Test
{
protected:
    OCProvisionDev_t *m_OwnList, *m_UnownList;
    OicSecAcl_t *m_Acl, *m_Acl1, *m_Acl2;
    PMCsdkHelper m_PMHelper;

    virtual void SetUp()
    {
        CommonUtil::runCommonTCSetUpPart();
        CommonUtil::killApp(JUSTWORKS_SERVER);
        CommonUtil::waitInSecond(DELAY_MEDIUM);
        CommonUtil::rmFile(CLIENT_DATABASE);
        CommonUtil::rmFile(JUSTWORKS_SERVER1_CBOR);
        CommonUtil::rmFile(JUSTWORKS_SERVER2_CBOR);
        CommonUtil::rmFile(CLIENT_CBOR);
        CommonUtil::waitInSecond(DELAY_LONG);
        CommonUtil::copyFile(JUSTWORKS_SERVER1_CBOR_BACKUP, JUSTWORKS_SERVER1_CBOR);
        CommonUtil::copyFile(JUSTWORKS_SERVER2_CBOR_BACKUP, JUSTWORKS_SERVER2_CBOR);
        CommonUtil::copyFile(CLIENT_CBOR_BACKUP, CLIENT_CBOR);
        CommonUtil::launchApp(JUSTWORKS_SERVER1);
        CommonUtil::launchApp(JUSTWORKS_SERVER2);
        CommonUtil::waitInSecond(DELAY_LONG);
        m_UnownList = NULL;
        m_OwnList = NULL;
    }

    virtual void TearDown()
    {
        CommonUtil::runCommonTCTearDownPart();
        CommonUtil::killApp(JUSTWORKS_SERVER);
    }
};

/**
 * @since           2015-12-08
 * @see             OCStackResult OCRegisterPersistentStorageHandler(OCPersistentStorage* persistentStorageHandler)
 * @see             OCStackResult OCInit(const char *ipAddr, uint16_t port, OCMode mode)
 * @see             OCStackResult OCInitPM(const char* dbPath)
 * @see             OCStackResult OCSetOwnerTransferCallbackData(OicSecOxm_t oxm, OTMCallbackData_t* callbackData)
 * @see             OCStackResult OCDoOwnershipTransfer(void* ctx, OCProvisionDev_t *targetDevices, OCProvisionResultCB resultCallback)
 * @see             OCStackResult OCDiscoverOwnedDevices(unsigned short timeout, OCProvisionDev_t **ppList)* @objective Test OCProvisionPairwiseDevices Negatively with Same 2 Device
 * @objective       Test OCProvisionPairwiseDevices negatively with pDev1 = pDev2
 * @target          OCStackResult OCProvisionPairwiseDevices(void* ctx, OicSecCredType_t type, size_t keySize, const OCProvisionDev_t *pDev1, OicSecAcl_t *pDev1Acl, const OCProvisionDev_t *pDev2, OicSecAcl_t *pDev2Acl, OCProvisionResultCB resultCallback)
 * @test_data       pDev1 = pDev2
 * @pre_condition   Start two justworks simulators
 * @procedure       1. call OCRegisterPersistentStorageHandler
 *                  2. call OCInit
 *                  3. call OCInitPM
 *                  4. call OCSetOwnerTransferCallbackData
 *                  5. call OCDiscoverUnownedDevices
 *                  6. call OCDoOwnershipTransfer
 *                  7. call OCDiscoverOwnedDevices
 *                  8. call OCProvisionPairwiseDevices
 * @post_condition  None
 * @expected        OCProvisionPairwiseDevices will return OC_STACK_INVALID_PARAM
 */
#if defined(__LINUX__)
TEST_F(PMCsdkTest_stc, ProvisionPairwiseDevicesSameDevices_EG_N)
{
    if (!m_PMHelper.initProvisionClient())
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    if (!m_PMHelper.discoverUnownedDevices(DISCOVERY_TIMEOUT,
                    &m_UnownList, OC_STACK_OK))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    if (!m_PMHelper.doOwnerShipTransfer((void*)g_ctx, &m_UnownList,
                    PMCsdkHelper::ownershipTransferCB, OC_STACK_OK))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    if(!m_PMHelper.discoverOwnedDevices(DISCOVERY_TIMEOUT, &m_OwnList, OC_STACK_OK))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    OCProvisionDev_t *device2 = m_OwnList->next;
    OicSecCredType_t type = SYMMETRIC_PAIR_WISE_KEY;
    size_t keySize = OWNER_PSK_LENGTH_128;

    if (!m_PMHelper.provisionPairwiseDevices((void*)ctxProvPairwise, type, keySize,
                    device2, NULL, device2, NULL, PMCsdkHelper::provisionPairwiseCB, OC_STACK_INVALID_PARAM))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }
}
#endif

/**
 * @since           2015-12-08
 * @see             OCStackResult OCRegisterPersistentStorageHandler(OCPersistentStorage* persistentStorageHandler)
 * @see             OCStackResult OCInit(const char *ipAddr, uint16_t port, OCMode mode)
 * @see             OCStackResult OCInitPM(const char* dbPath)
 * @see             OCStackResult OCSetOwnerTransferCallbackData(OicSecOxm_t oxm, OTMCallbackData_t* callbackData)
 * @see             OCStackResult OCDoOwnershipTransfer(void* ctx, OCProvisionDev_t *targetDevices, OCProvisionResultCB resultCallback)
 * @see             OCStackResult OCDiscoverOwnedDevices(unsigned short timeout, OCProvisionDev_t **ppList)* @objective Test OCProvisionPairwiseDevices Negatively with Same 2 Device
 * @objective       Test OCProvisionPairwiseDevices negatively calling it multiple time
 * @target          OCStackResult OCProvisionPairwiseDevices(void* ctx, OicSecCredType_t type, size_t keySize, const OCProvisionDev_t *pDev1, OicSecAcl_t *pDev1Acl, const OCProvisionDev_t *pDev2, OicSecAcl_t *pDev2Acl, OCProvisionResultCB resultCallback)
 * @test_data       call OCProvisionPairwiseDevices second time
 * @pre_condition   Start two justworks simulators
 * @procedure       1. call OCRegisterPersistentStorageHandler
 *                  2. call OCInit
 *                  3. call OCInitPM
 *                  4. call OCSetOwnerTransferCallbackData
 *                  5. call OCDiscoverUnownedDevices
 *                  6. call OCDoOwnershipTransfer
 *                  7. call OCDiscoverOwnedDevices
 *                  8. call OCProvisionPairwiseDevices
 *                  9. call OCProvisionPairwiseDevices
 * @post_condition  None
 * @expected        OCProvisionPairwiseDevices will return OC_STACK_INVALID_PARAM
 */
#if defined(__LINUX__)
TEST_F(PMCsdkTest_stc, ProvisionPairwiseDevicesMultipleTime_EG_N)
{
    if (!m_PMHelper.initProvisionClient())
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    if (!m_PMHelper.discoverUnownedDevices(DISCOVERY_TIMEOUT,
                    &m_UnownList, OC_STACK_OK))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    if (!m_PMHelper.doOwnerShipTransfer((void*)g_ctx, &m_UnownList,
                    PMCsdkHelper::ownershipTransferCB, OC_STACK_OK))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    if(!m_PMHelper.discoverOwnedDevices(DISCOVERY_TIMEOUT, &m_OwnList, OC_STACK_OK))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    OCProvisionDev_t *device1 = m_OwnList;
    OCProvisionDev_t *device2 = m_OwnList->next;
    OicSecCredType_t type = SYMMETRIC_PAIR_WISE_KEY;
    size_t keySize = OWNER_PSK_LENGTH_128;

    if (!m_PMHelper.provisionPairwiseDevices((void*)ctxProvPairwise, type, keySize,
                    device1, NULL, device2, NULL, PMCsdkHelper::provisionPairwiseCB, OC_STACK_OK))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    if (!m_PMHelper.provisionPairwiseDevices((void*)ctxProvPairwise, type, keySize,
            device1, NULL, device2, NULL, PMCsdkHelper::provisionPairwiseCB, OC_STACK_INVALID_PARAM))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }
}
#endif

/**
 * @since           2015-02-19
 * @see             OCStackResult OCRegisterPersistentStorageHandler(OCPersistentStorage* persistentStorageHandler)
 * @see             OCStackResult OCInit(const char *ipAddr, uint16_t port, OCMode mode)
 * @see             OCStackResult OCInitPM(const char* dbPath)
 * @see             OCStackResult OCSetOwnerTransferCallbackData(OicSecOxm_t oxm, OTMCallbackData_t* callbackData)
 * @see             OCStackResult OCDoOwnershipTransfer(void* ctx, OCProvisionDev_t *targetDevices, OCProvisionResultCB resultCallback)
 * @see             OCStackResult OCDiscoverOwnedDevices(unsigned short timeout, OCProvisionDev_t **ppList)
 * @objective       Test OCUnlinkDevices  negatively trying to remove client device.
 * @target          OCStackResult OCUnlinkDevices(void* ctx, const OCProvisionDev_t* pTargetDev1, const OCProvisionDev_t* pTargetDev2, OCProvisionResultCB resultCallback)
 * @test_data       pTargetDev1 = client device
 * @pre_condition   Start two justworks simulators
 * @procedure       1. call OCRegisterPersistentStorageHandler
 *                  2. call OCInit
 *                  3. call OCInitPM
 *                  4. call OCSetOwnerTransferCallbackData
 *                  5. call OCDiscoverUnownedDevices
 *                  6. call OCDoOwnershipTransfer
 *                  7. call OCDiscoverOwnedDevices
 *                  8. call OCUnlinkDevices
 * @post_condition  None
 * @expected        OCUnlinkDevices will return OC_STACK_INVALID_PARAM
 */
#if defined(__LINUX__)
TEST_F(PMCsdkTest_stc, UnlinkDevicesClientDev_EG_N)
{
    if (!m_PMHelper.initProvisionClient())
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    if (!m_PMHelper.discoverUnownedDevices(DISCOVERY_TIMEOUT,
                    &m_UnownList, OC_STACK_OK))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    if (!m_PMHelper.doOwnerShipTransfer((void*)g_ctx, &m_UnownList,
                    PMCsdkHelper::ownershipTransferCB, OC_STACK_OK))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    if(!m_PMHelper.discoverOwnedDevices(DISCOVERY_TIMEOUT, &m_OwnList, OC_STACK_OK))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    OCProvisionDev_t *device1 = m_OwnList->next->next;
    OCProvisionDev_t *device2 = m_OwnList;

    if (!m_PMHelper.unlinkDevices((void*)ctxUnlinkDevice, device1, device2,
                    PMCsdkHelper::unlinkDevicesCB, OC_STACK_INVALID_PARAM))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }
}
#endif

/**
 * @since           2015-02-19
 * @see             OCStackResult OCRegisterPersistentStorageHandler(OCPersistentStorage* persistentStorageHandler)
 * @see             OCStackResult OCInit(const char *ipAddr, uint16_t port, OCMode mode)
 * @see             OCStackResult OCInitPM(const char* dbPath)
 * @see             OCStackResult OCSetOwnerTransferCallbackData(OicSecOxm_t oxm, OTMCallbackData_t* callbackData)
 * @see             OCStackResult OCDoOwnershipTransfer(void* ctx, OCProvisionDev_t *targetDevices, OCProvisionResultCB resultCallback)
 * @see             OCStackResult OCDiscoverOwnedDevices(unsigned short timeout, OCProvisionDev_t **ppList)
 * @objective       Test OCUnlinkDevices  negatively trying to unlink same device.
 * @target          OCStackResult OCUnlinkDevices(void* ctx, const OCProvisionDev_t* pTargetDev1, const OCProvisionDev_t* pTargetDev2, OCProvisionResultCB resultCallback)
 * @test_data       pTargetDev1 = pTargetDev2
 * @pre_condition   Start two justworks simulators
 * @procedure       1. call OCRegisterPersistentStorageHandler
 *                  2. call OCInit
 *                  3. call OCInitPM
 *                  4. call OCSetOwnerTransferCallbackData
 *                  5. call OCDiscoverUnownedDevices
 *                  6. call OCDoOwnershipTransfer
 *                  7. call OCDiscoverOwnedDevices
 *                  8. call OCUnlinkDevices
 * @post_condition  None
 * @expected        OCUnlinkDevices will return OC_STACK_INVALID_PARAM
 */
#if defined(__LINUX__)
TEST_F(PMCsdkTest_stc, UnlinkDevicesSameDevicesDev_EG_N)
{
    if (!m_PMHelper.initProvisionClient())
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    if (!m_PMHelper.discoverUnownedDevices(DISCOVERY_TIMEOUT,
                    &m_UnownList, OC_STACK_OK))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    if (!m_PMHelper.doOwnerShipTransfer((void*)g_ctx, &m_UnownList,
                    PMCsdkHelper::ownershipTransferCB, OC_STACK_OK))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    if(!m_PMHelper.discoverOwnedDevices(DISCOVERY_TIMEOUT, &m_OwnList, OC_STACK_OK))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    OCProvisionDev_t *device1 = m_OwnList;

    if (!m_PMHelper.unlinkDevices((void*)ctxUnlinkDevice, device1, device1,
                    PMCsdkHelper::unlinkDevicesCB, OC_STACK_INVALID_PARAM))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }
}
#endif

/**
 * @since           2015-03-12
 * @see             OCStackResult OCRegisterPersistentStorageHandler(OCPersistentStorage* persistentStorageHandler)
 * @see             OCStackResult OCInit(const char *ipAddr, uint16_t port, OCMode mode)
 * @see             OCStackResult OCInitPM(const char* dbPath)
 * @see             OCStackResult OCSetOwnerTransferCallbackData(OicSecOxm_t oxm, OTMCallbackData_t* callbackData)
 * @see             OCStackResult OCDoOwnershipTransfer(void* ctx, OCProvisionDev_t *targetDevices, OCProvisionResultCB resultCallback)
 * @see             OCStackResult OCDiscoverOwnedDevices(unsigned short timeout, OCProvisionDev_t **ppList)* @objective Test OCProvisionDirectPairing positively.
 * @target          OCStackResult OCProvisionDirectPairing(void* ctx, const OCProvisionDev_t *selectedDeviceInfo, OicSecPconf_t *pconf, OCProvisionResultCB resultCallback)
 * @objective       Test OCProvisionDirectPairing negatively for client device.
 * @test_data       selectedDeviceInfo = client device
 * @pre_condition   Start two justworks simulators
 * @procedure       1. call OCRegisterPersistentStorageHandler
 *                  2. call OCInit
 *                  3. call OCInitPM
 *                  4. call OCSetOwnerTransferCallbackData
 *                  5. call OCDiscoverUnownedDevices
 *                  6. call OCDoOwnershipTransfer
 *                  7. call OCDiscoverOwnedDevices
 *                  8. call OCProvisionDirectPairing
 * @post_condition  None
 * @expected        OCProvisionDirectPairing will return OC_STACK_UNAUTHORIZED_REQ
 */
#if defined(__LINUX__)
TEST_F(PMCsdkTest_stc, OCProvisionDirectPairingClientDevice_EG_N)
{
    if (!m_PMHelper.initProvisionClient())
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    if (!m_PMHelper.discoverUnownedDevices(DISCOVERY_TIMEOUT,
                    &m_UnownList, OC_STACK_OK))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    if (!m_PMHelper.doOwnerShipTransfer((void*)g_ctx, &m_UnownList,
                    m_PMHelper.ownershipTransferCB, OC_STACK_OK))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    if(!m_PMHelper.discoverOwnedDevices(DISCOVERY_TIMEOUT, &m_OwnList, OC_STACK_OK))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    printDevList(m_OwnList);
    OCProvisionDev_t *device1 = m_OwnList->next->next;

    OicSecPconf_t pconf;
    memset(&pconf, 0, sizeof(OicSecPconf_t));
    pconf.edp = true;

    // set default supported PRM types
    pconf.prmLen = sizeof(SUPPORTED_PRMS)/sizeof(OicSecPrm_t);
    pconf.prm = (OicSecPrm_t *)OICCalloc(pconf.prmLen, sizeof(OicSecPrm_t));

    if(pconf.prm)
    {
        for (size_t i=0; i<pconf.prmLen; i++)
        {
            pconf.prm[i] = SUPPORTED_PRMS[i];
        }
    }

    // set default pin
    memcpy(pconf.pin.val, DEFAULT_DP_PROVSIONING_PIN, DP_PIN_LENGTH);

    // set default pdacl
    pconf.pdacls = createPdAcl(DEVICE_INDEX_ONE);

    if (!m_PMHelper.proivisioningDirectPairing(g_ctx, device1,
                    &pconf, m_PMHelper.provisionDPCB, OC_STACK_UNAUTHORIZED_REQ))
    {

        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }
}
#endif

/**
 * @since           2016-02-19
 * @see             OCStackResult OCRegisterPersistentStorageHandler(OCPersistentStorage* persistentStorageHandler)
 * @see             OCStackResult OCInit(const char *ipAddr, uint16_t port, OCMode mode)
 * @see             OCStackResult OCInitPM(const char* dbPath)
 * @see             OCStackResult OCSetOwnerTransferCallbackData(OicSecOxm_t oxm, OTMCallbackData_t* callbackData)
 * @see             OCStackResult OCDoOwnershipTransfer(void* ctx, OCProvisionDev_t *targetDevices, OCProvisionResultCB resultCallback)
 * @see             OCStackResult OCDiscoverOwnedDevices(unsigned short timeout, OCProvisionDev_t **ppList)
 * @objective       Test OCRemoveDevice negatively with client dev as OCRemoveDevice.
 * @target          OCStackResult OCRemoveDevice(void* ctx, unsigned short waitTimeForOwnedDeviceDiscovery, const OCProvisionDev_t* pTargetDev, OCProvisionResultCB resultCallback)
 * @test_data       pTargetDev = client device
 * @pre_condition   Start two justworks simulators
 * @procedure       1. call OCRegisterPersistentStorageHandler
 *                  2. call OCInit
 *                  3. call OCInitPM
 *                  4. call OCSetOwnerTransferCallbackData
 *                  5. call OCDiscoverUnownedDevices
 *                  6. call OCDoOwnershipTransfer
 *                  7. call OCDiscoverOwnedDevices
 *                  8. call OCRemoveDevice
 * @post_condition  None
 * @expected        OCRemoveDevice will return OC_STACK_INVALID_PARAM
 */
#if defined(__LINUX__)
TEST_F(PMCsdkTest_stc, RemoveDeviceClientDev_EG_N)
{
    if (!m_PMHelper.initProvisionClient())
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    if (!m_PMHelper.discoverUnownedDevices(DISCOVERY_TIMEOUT,
                    &m_UnownList, OC_STACK_OK))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    if (!m_PMHelper.doOwnerShipTransfer((void*)g_ctx, &m_UnownList,
                    PMCsdkHelper::ownershipTransferCB, OC_STACK_OK))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    if(!m_PMHelper.discoverOwnedDevices(DISCOVERY_TIMEOUT, &m_OwnList, OC_STACK_OK))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    OCProvisionDev_t *device1 = m_OwnList->next->next;

    if (!m_PMHelper.removeDevice((void*)ctxRemoveDevice, DISCOVERY_TIMEOUT, device1,
                    PMCsdkHelper::removeDeviceCB, OC_STACK_INVALID_PARAM))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }
}
#endif

/**
 * @since           2016-02-19
 * @see             OCStackResult OCRegisterPersistentStorageHandler(OCPersistentStorage* persistentStorageHandler)
 * @see             OCStackResult OCInit(const char *ipAddr, uint16_t port, OCMode mode)
 * @see             OCStackResult OCInitPM(const char* dbPath)
 * @see             OCStackResult OCSetOwnerTransferCallbackData(OicSecOxm_t oxm, OTMCallbackData_t* callbackData)
 * @see             OCStackResult OCDoOwnershipTransfer(void* ctx, OCProvisionDev_t *targetDevices, OCProvisionResultCB resultCallback)
 * @see             OCStackResult OCDiscoverOwnedDevices(unsigned short timeout, OCProvisionDev_t **ppList)
 * @see             OCStackResult OCProvisionPairwiseDevices(void* ctx, OicSecCredType_t type, size_t keySize, const OCProvisionDev_t *pDev1, OicSecAcl_t *pDev1Acl, const OCProvisionDev_t *pDev2, OicSecAcl_t *pDev2Acl, OCProvisionResultCB resultCallback)
 * @objective       Test OCGetLinkedStatus positively with  unformatted uuidOfDevice
 * @target          OCStackResult OCGetLinkedStatus(const OicUuid_t* uuidOfDevice, OCUuidList_t** uuidList, size_t* numOfDevices)
 * @test_data       OCGetLinkedStatus with a reference of unformatted Device ID
 * @pre_condition   Start two justworks simulators
 * @procedure       1. call OCRegisterPersistentStorageHandler
 *                  2. call OCInit
 *                  3. call OCInitPM
 *                  4. call OCSetOwnerTransferCallbackData
 *                  5. call OCDiscoverUnownedDevices
 *                  6. call OCDoOwnershipTransfer
 *                  7. call OCDiscoverOwnedDevices
 *                  8. call OCProvisionPairwiseDevices
 *                  9. call OCGetLinkedStatus
 * @post_condition  None
 * @expected        OCGetLinkedStatus will return OC_STACK_INVALID_PARAM
 */
#if defined(__LINUX__)
TEST_F(PMCsdkTest_stc, OCGetLinkedStatusUuid_URSV_N)
{
    if (!m_PMHelper.initProvisionClient())
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    if (!m_PMHelper.discoverUnownedDevices(DISCOVERY_TIMEOUT,
                    &m_UnownList, OC_STACK_OK))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    if (!m_PMHelper.doOwnerShipTransfer((void*)g_ctx, &m_UnownList,
                    PMCsdkHelper::ownershipTransferCB, OC_STACK_OK))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    if(!m_PMHelper.discoverOwnedDevices(DISCOVERY_TIMEOUT, &m_OwnList, OC_STACK_OK))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    OCProvisionDev_t *device1 = m_OwnList;
    OCProvisionDev_t *device2 = m_OwnList->next;
    m_Acl1 = createAcl(DEVICE_INDEX_ONE, &m_OwnList);
    m_Acl2 = createAcl(DEVICE_INDEX_TWO, &m_OwnList);
    OicSecCredType_t type = SYMMETRIC_PAIR_WISE_KEY;
    size_t keySize = OWNER_PSK_LENGTH_128;

    if (!m_PMHelper.provisionPairwiseDevices((void*)ctxProvPairwise, type, keySize,
                    device1, m_Acl1, device2, m_Acl2, PMCsdkHelper::provisionPairwiseCB, OC_STACK_OK))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }

    OicUuid_t uuidOfDevice;

    for(int i=0; i <= 15; i++)
    {
        uuidOfDevice.id[i]= 1;
    }

    OCUuidList_t* uuidList = NULL;
    size_t numOfDevices;

    if (!m_PMHelper.getLinkedStatus(&uuidOfDevice, uuidList, &numOfDevices, OC_STACK_INVALID_PARAM))
    {
        SET_FAILURE(m_PMHelper.getFailureMessage());
        return;
    }
}
#endif
