// Copyright (c) 2014-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "validation.h"
#include "net.h"

#include "test/test_bitcoin.h"

#include <boost/signals2/signal.hpp>
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(main_tests, TestingSetup)

BOOST_AUTO_TEST_CASE(block_subsidy_test)
{
    const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
    const auto& consensus = chainParams->GetConsensus();

    BOOST_CHECK_EQUAL(GetBlockSubsidy(0, consensus), COIN);
    BOOST_CHECK_EQUAL(GetBlockSubsidy(1, consensus), 1 * COIN);
    BOOST_CHECK_EQUAL(GetBlockSubsidy(2, consensus), COIN);
    BOOST_CHECK_EQUAL(GetBlockSubsidy(consensus.nSubsidyHalvingInterval + 1, consensus), COIN / 2);
    BOOST_CHECK_EQUAL(GetBlockSubsidy(2 * consensus.nSubsidyHalvingInterval + 1, consensus), COIN / 4);
    const int offset = 2 * consensus.nSubsidyHalvingInterval;
    const int nHeight = offset + consensus.nSubsidyHalvingIntervalPhase2 + 1;
    BOOST_CHECK_EQUAL(GetBlockSubsidy(nHeight, consensus), COIN / 8);
}

BOOST_AUTO_TEST_CASE(subsidy_limit_test)
{
    const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
    CAmount nSum = 0;
    for (int nHeight = 0; nHeight < 23500; nHeight += 100) {
        CAmount nSubsidy = GetBlockSubsidy(nHeight, chainParams->GetConsensus());
        BOOST_CHECK(nSubsidy <= COIN);
        nSum += nSubsidy * 100;
        BOOST_CHECK(MoneyRange(nSum));
    }
    BOOST_CHECK_EQUAL(nSum, 23500 * COIN);
}

bool ReturnFalse() { return false; }
bool ReturnTrue() { return true; }

BOOST_AUTO_TEST_CASE(test_combiner_all)
{
    boost::signals2::signal<bool (), CombinerAll> Test;
    BOOST_CHECK(Test());
    Test.connect(&ReturnFalse);
    BOOST_CHECK(!Test());
    Test.connect(&ReturnTrue);
    BOOST_CHECK(!Test());
    Test.disconnect(&ReturnFalse);
    BOOST_CHECK(Test());
    Test.disconnect(&ReturnTrue);
    BOOST_CHECK(Test());
}
BOOST_AUTO_TEST_SUITE_END()