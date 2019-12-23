// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include "chainparamsseeds.h"
#include "pow.h"
#include "arith_uint256.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 521142271 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
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
 * Build the Yespower genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 CBlock(hash=d62c5827472649d4152596082bde1abc2fc530da75fee7695aa07925ed3d4d0a, yespowerpow=000a599884f027634adca2556787296ae7a5bb592da4df6cd63f1a6cec56e9eb, ver=0x00000001, hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000, hashMerkleRoot=85215868731227d55b927c43466028c50e0331a3fb4bd851637aa6b86910aacc, nTime=1576458645, nBits=1f0fffff, nNonce=8510, vtx=1)
  CTransaction(hash=8521586873, ver=1, vin.size=1, vout.size=1, nLockTime=0)
    CTxIn(COutPoint(0000000000, 4294967295), coinbase 04ffff0f1f01040b31362f4465632f32303139)
    CScriptWitness()
    CTxOut(nValue=50.00000000, scriptPubKey=4104c46347dee692f947297900efc7)
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "23/Dec/2019";
    const CScript genesisOutputScript = CScript() << ParseHex("04c46347dee692f947297900efc713e2b66fea90075ae9397b519948c8fccd19540d6d13bf7fd66813723458b265bef2062a0a71623f0e8176bc39edc10f30d0c4") << OP_CHECKSIG;
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
        consensus.nSubsidyHalvingInterval = 2100000;
        consensus.BIP34Height = 0;
    //    consensus.BIP34Hash   = uint256S("0xd62c5827472649d4152596082bde1abc2fc530da75fee7695aa07925ed3d4d0a");
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.powLimit    = uint256S("0x000fffff00000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60;
        consensus.nPowTargetSpacing  = 60;           
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 19160;  // 95% of 20160
        consensus.nMinerConfirmationWindow = 20160;    // nPowTargetTimespan / nPowTargetSpacing

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1573869600; // November 16th, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1605492000; // November 16th, 2020

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1573869600; // November 16th, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1605492000; // November 16th, 2020

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork =  uint256S ("0x0000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S ("0x0000000000000000000000000000000000000000000000000000000000000000"); // 0

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x76;
        pchMessageStart[3] = 0x43;
        pchMessageStart[1] = 0x09;
        pchMessageStart[2] = 0x82;
        nDefaultPort       = 9222;
        nPruneAfterHeight  = 1000000;

   //     genesis = CreateGenesisBlock(1577104170, 8510, 0x1f0fffff, 1, 50 * COIN);

        uint32_t nTime = 1577104170;
        uint32_t nNonce = 0;

        if (nNonce == 0) {
           while (UintToArith256(genesis.GetHashYespower()) > UintToArith256(consensus.powLimit)) {
              nNonce++;
              genesis = CreateGenesisBlock(nTime, nNonce, 0x1f0fffff, 1, 50 * COIN);
              if (nNonce % 128 == 0) printf("\rnonce %08x", nNonce);
           }
           printf("\n%s\n", genesis.ToString().c_str());
        }
        genesis = CreateGenesisBlock(nTime, nNonce, 0x1f0fffff, 1, 50 * COIN);

        consensus.hashGenesisBlock = genesis.GetHash();

   //     assert (consensus.hashGenesisBlock == uint256S("0xd62c5827472649d4152596082bde1abc2fc530da75fee7695aa07925ed3d4d0a"));
   //     assert (genesis.hashMerkleRoot == uint256S("0x85215868731227d55b927c43466028c50e0331a3fb4bd851637aa6b86910aacc"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,76);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,63);
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));
        vSeeds.clear();
    //    vSeeds.emplace_back ("node.intercoin.com", false);

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fMiningRequiresPeers = true;

        checkpointData = (CCheckpointData) {
            {
     //           { 0,     uint256S("0xd62c5827472649d4152596082bde1abc2fc530da75fee7695aa07925ed3d4d0a") }
            }
        };

        chainTxData = ChainTxData {
            // Data as of genesis block.
            1577104170,  // * UNIX timestamp of last known number of transactions
            0,        // * total number of transactions between genesis and that timestamp
                         //   (the tx=... number in the SetBestChain debug.log lines)
            0.0 // * estimated number of transactions per second after that timestamp
        };
    }
};

/**
 * Testnet (v1)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 2100000;

        consensus.BIP34Height = 0;
 //       consensus.BIP34Hash   = uint256S("0xd62c5827472649d4152596082bde1abc2fc530da75fee7695aa07925ed3d4d0a");
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.powLimit    = uint256S("0x000fffff00000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60;
        consensus.nPowTargetSpacing  = 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 15120;  // 75% of 20160
        consensus.nMinerConfirmationWindow = 20160;    // nPowTargetTimespan / nPowTargetSpacing

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1573869600; // November 16th, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1605492000; // November 16th, 2020

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1573869600; // November 16th, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1605492000; // November 16th, 2020

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork =  uint256S ("0x0000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S ("0x0000000000000000000000000000000000000000000000000000000000000000"); // 0

        pchMessageStart[0] = 0x56;
        pchMessageStart[1] = 0x36;
        pchMessageStart[2] = 0x60;
        pchMessageStart[3] = 0x39;
        nDefaultPort       = 19222;
        nPruneAfterHeight  = 1000;

    //    genesis = CreateGenesisBlock(1577104170, 8510, 0x1f0fffff, 1, 50 * COIN);

        uint32_t nTime = 1577104170;
        uint32_t nNonce = 0;

        if (nNonce == 0) {
           while (UintToArith256(genesis.GetHashYespower()) > UintToArith256(consensus.powLimit)) {
              nNonce++;
              genesis = CreateGenesisBlock(nTime, nNonce, 0x1f0fffff, 1, 50 * COIN);
              if (nNonce % 128 == 0) printf("\rnonce %08x", nNonce);
           }
           printf("\n%s\n", genesis.ToString().c_str());
        }
        genesis = CreateGenesisBlock(nTime, nNonce, 0x1f0fffff, 1, 50 * COIN);

        consensus.hashGenesisBlock = genesis.GetHash();

   //     assert (consensus.hashGenesisBlock == uint256S("0xd62c5827472649d4152596082bde1abc2fc530da75fee7695aa07925ed3d4d0a"));
   //     assert (genesis.hashMerkleRoot == uint256S("0x85215868731227d55b927c43466028c50e0331a3fb4bd851637aa6b86910aacc"));

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));
   //     vSeeds.emplace_back ("node.intercoin.com", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fMiningRequiresPeers = true;

        checkpointData = (CCheckpointData) {
            {
	//	{0, uint256S("0xd62c5827472649d4152596082bde1abc2fc530da75fee7695aa07925ed3d4d0a")}
            }
        };

        chainTxData = ChainTxData {
            /* data as of:  */
            1576458645,
            0,
            0.0
        };
    }
};

/**
 * Regression test
CBlock(hash=d3aa89f7cd1c6ecb99045f4fc4ec6ffe8fe6f159e4c8ee33f31572d0c660bbc9, yespowerpow=0b0ad48a0445daed19621fe44b0a590621a2b0f99f37b8441b7d78a9736e0bee, ver=0x00000001, hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000, hashMerkleRoot=85215868731227d55b927c43466028c50e0331a3fb4bd851637aa6b86910aacc, nTime=1576458645, nBits=200fffff, nNonce=30, vtx=1)
  CTransaction(hash=8521586873, ver=1, vin.size=1, vout.size=1, nLockTime=0)
    CTxIn(COutPoint(0000000000, 4294967295), coinbase 04ffff0f1f01040b31362f4465632f32303139)
    CScriptWitness()
    CTxOut(nValue=50.00000000, scriptPubKey=4104c46347dee692f947297900efc7)
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 35;

        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 0; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 0; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("0fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60;
        consensus.nPowTargetSpacing  = 60;
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
        consensus.nMinimumChainWork  = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0x44;
        pchMessageStart[1] = 0x25;
        pchMessageStart[2] = 0x71;
        pchMessageStart[3] = 0x29;
        nDefaultPort       = 19444;
        nPruneAfterHeight  = 1000;

    //    genesis = CreateGenesisBlock(1577104170, 30, 0x200fffff, 1, 50 * COIN);

        uint32_t nTime = 1577104170;
        uint32_t nNonce = 0;

        if (nNonce == 0) {
           while (UintToArith256(genesis.GetHashYespower()) > UintToArith256(consensus.powLimit)) {
              nNonce++;
              genesis = CreateGenesisBlock(nTime, nNonce, 0x200fffff, 1, 50 * COIN);
              if (nNonce % 128 == 0) printf("\rnonce %08x", nNonce);
           }
           printf("\n%s\n", genesis.ToString().c_str());
        }
        genesis = CreateGenesisBlock(nTime, nNonce, 0x200fffff, 1, 50 * COIN);

        consensus.hashGenesisBlock = genesis.GetHash();

   //     assert (consensus.hashGenesisBlock == uint256S("0xd3aa89f7cd1c6ecb99045f4fc4ec6ffe8fe6f159e4c8ee33f31572d0c660bbc9"));
   //     assert (genesis.hashMerkleRoot == uint256S("0x85215868731227d55b927c43466028c50e0331a3fb4bd851637aa6b86910aacc"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fMiningRequiresPeers = false;

        checkpointData = (CCheckpointData) {
            {
                // {0, uint256S("00")},
            }
        };

        chainTxData = ChainTxData {
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
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
