// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Mars Polar Lander";
    const CScript genesisOutputScript = CScript() << ParseHex("040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 500000;
        consensus.BIP34Height = 125;
        consensus.BIP34Hash = uint256S("0xf3fccb8e672ee83687262bb1dfbe3ee7170206c52bdb7fef8d95df6bf421389c");
        consensus.BIP65Height = 175; // 85a64d4aa5dedee5c1c1ff4b929c3282d0077b7e9f342a3c35962bf65c4f57f7
        consensus.BIP66Height = 150; // f9815a13c7f1d694c1683f3fb1989f431c0e4f064eb536f1d6c70d176722037a
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // 3.5 days
        consensus.nPowTargetSpacing = 5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 3427; // 85% of 4032
        consensus.nMinerConfirmationWindow = 4032; // nPowTargetTimespan / nPowTargetSpacing * 4
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1514795449; // January 1, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1546264820; // December 31, 2018

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1546331415; // January 01, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1609455599; // January 31st, 2020

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1546331415; // January 01, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1609455599; // January 31st, 2020

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000013701370");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x6cc08b49ab0c2b1e400b0fb29ee82c86dc79eab3ab1f80bea2d339403a7aa491"); // height : 225

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xa6;
        pchMessageStart[1] = 0xe3;
        pchMessageStart[2] = 0xb6;
        pchMessageStart[3] = 0xce;
        nDefaultPort = 7333;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(944221660, 599575, 0x1e0ffff0, 1, 75 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xf1f797af6649ff63a9acab412d8d44bdedd9930b692c3d387c8fe04b34f5aa3b"));
        assert(genesis.hashMerkleRoot == uint256S("0xc4e56452ef813eb78c89d5ed2594d28104126c404538339897a0a7f72fe174d7"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.emplace_back("seed.a12coin.org", true);
//        vSeeds.emplace_back("seed1.a12coin.org", true);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,23);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,75);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,76);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,23+128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x45, 0x6C, 0x6C, 0x61};
        base58Prefixes[EXT_SECRET_KEY] = {0x45, 0x6C, 0x73, 0x61};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            {
                {     0, uint256S("0xf1f797af6649ff63a9acab412d8d44bdedd9930b692c3d387c8fe04b34f5aa3b")},
                {   150, uint256S("0xf9815a13c7f1d694c1683f3fb1989f431c0e4f064eb536f1d6c70d176722037a")},
                {   300, uint256S("0x13ff2b91e8553e70216745ccc80f556b938e950da278a321d82eddd97a393f48")},

            }
        };

        chainTxData = ChainTxData{
            // Data as of block 11eb46105ea10fb0d113757024c036881624602fb2b7e438c8138a2d61f8c120 (height 375).
            1639602001, // * UNIX timestamp of last known number of transactions
            0,          // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0.0         // * estimated number of transactions per second after that timestamp
        };
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 500000;
        consensus.BIP34Height = 75;
        consensus.BIP34Hash = uint256S("0xc786c4e2e345943c6fa24f672d1db0f6065815bcab39db5ec7160e1ee6555af8");
        consensus.BIP65Height = 95; // 964172bf398c3c276e77a05abade88f668348df10d8323958d25e7c61d369a23
        consensus.BIP66Height = 85; // ccaa9cc20abc68cd340a635931f7c09b4fd2e1159213b39fa9fcd8be6e5f8014
        consensus.powLimit = uint256S("0x0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 2 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1514795449; // January 1, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1546264820; // December 31, 2018

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1546331415; // January 01, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1609455599; // January 31st, 2020

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1546331415; // January 01, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1609455599; // January 31st, 2020

        // The best chain should have at least this much work.
       consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000001500150");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x837ce40bf685d4e8ccea69c0887dfe3aa9958eb157a3660d684d48b6c80622da"); // height : 275

        pchMessageStart[0] = 0xa6;
        pchMessageStart[1] = 0xdd;
        pchMessageStart[2] = 0xe4;
        pchMessageStart[3] = 0xb3;
        nDefaultPort = 27335;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1517625873, 92451, 0x1e0ffff0, 1, 75 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xd25368dac38132f05d3488b595f9948d2837b7fdcd0a91d0a8b81c73d95eca56"));
        assert(genesis.hashMerkleRoot == uint256S("0xc4e56452ef813eb78c89d5ed2594d28104126c404538339897a0a7f72fe174d7"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("testnet-seed.a12coin.org", true);
//        vSeeds.emplace_back("testnet-seed1.a12coin.org", true);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,83);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,138);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,83+128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x45, 0x6C, 0x6C, 0x61};
        base58Prefixes[EXT_SECRET_KEY] = {0x45, 0x6C, 0x73, 0x61};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            {
                {     0, uint256S("0xd25368dac38132f05d3488b595f9948d2837b7fdcd0a91d0a8b81c73d95eca56")},
                {   300, uint256S("0xf6220fe679f5464aaf5912ae53fbbd2b829c052493ac7cdefc94d22f722090a1")},
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 1f8d872cfb254aadac8c2842ff055f8f3042c9a4ab12d302cd6783a04db651b6 (height 325)
            1639452602,
            0,
            0.0
        };

     }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 2 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0x4a;
        pchMessageStart[1] = 0xe0;
        pchMessageStart[2] = 0x9a;
        pchMessageStart[3] = 0x2f;
        nDefaultPort = 27444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1514947473, 2, 0x207fffff, 1, 75 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x12f626d0932c8c7930c1571e7c6439cea9d7462dd63a1882a118ea93113fa764"));
        assert(genesis.hashMerkleRoot == uint256S("0xc4e56452ef813eb78c89d5ed2594d28104126c404538339897a0a7f72fe174d7"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData) {
            {
                {0, uint256S("12f626d0932c8c7930c1571e7c6439cea9d7462dd63a1882a118ea93113fa764")},
            }
        };

        chainTxData = ChainTxData{
            1514947473,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,111+128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
