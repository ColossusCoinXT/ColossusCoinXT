// Copyright (c) 2018 The COLX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "context.h"
#include "bootstrapmodel.h"
#include "autoupdatemodel.h"
#include "utilstrencodings.h"
#include "timedata.h"
#include "util.h"

#include <memory>
#include <stdexcept>

using namespace std;

static unique_ptr<CContext> context_;

void CreateContext()
{
    if (context_)
        throw runtime_error("context has already been initialized, revise your code");
    else
        context_.reset(new CContext);
}

void ReleaseContext()
{
    context_.reset();
}

CContext& GetContext()
{
    if (!context_)
        throw runtime_error("context is not initialized");
    else
        return *context_;
}

CContext::CContext()
{
    nStartupTime_ = GetAdjustedTime();
    banAddrConsensus_.insert(make_pair("DSesymccyAQr6LjGLCHsvHzE41uKMk86XS", 0)); // Cryptopia
    banAddrConsensus_.insert(make_pair("D5cNN2DThi3UUwS1hhedfx1UreXjMuxZPp", 1297585)); // spammer
    banAddrConsensus_.insert(make_pair("DJB3pXt9Xuz7UTwPg4R8YtXB75gpNmkErD", 1297585)); // spammer
    banAddrConsensus_.insert(make_pair("DD7AMhT66bK3TpDUKdq6AsUBj8gt6pSd2o", 3634800));
    banAddrConsensus_.insert(make_pair("DRsR58hnZuver2E1wBtW8bTbRbHPHdJFvo", 3634800));
    banAddrConsensus_.insert(make_pair("D9f4VDKvmGKyf5S8ZkWGkxTcFZ83H5V2pn", 3634800));
    banAddrConsensus_.insert(make_pair("DFRMDgrmgSxLvBQjTQDhBtmfHjzjAByeVU", 3634800));
    banAddrConsensus_.insert(make_pair("DKDRXTuUcXJwuH7cjuVx9fc1sfqUYMhjq3", 3634800));
    banAddrConsensus_.insert(make_pair("DA9bAdRYEDjf1cr2nh3TyHAjkVcBMsxCk3", 3634800));
    banAddrConsensus_.insert(make_pair("DAvjsb1TvMRL7GEwB5uykR2w3UPFKsNKSP", 3634800));
    banAddrConsensus_.insert(make_pair("DRA9XzHxVEpz5yKQbMNJDSLYpTDY7TmuEr", 3634800));
}

CContext::~CContext() {}

int64_t CContext::GetStartupTime() const
{
    return nStartupTime_;
}

void CContext::SetStartupTime(int64_t nTime)
{
    nStartupTime_ = nTime;
}

BootstrapModelPtr CContext::GetBootstrapModel()
{
    if (!bootstrapModel_)
        bootstrapModel_.reset(new BootstrapModel);

    return bootstrapModel_;
}

AutoUpdateModelPtr CContext::GetAutoUpdateModel()
{
    if (!autoupdateModel_)
        autoupdateModel_.reset(new AutoUpdateModel);

    return autoupdateModel_;
}

void CContext::AddAddressToBan(
        const std::vector<std::string>& mempool,
        const std::vector<std::string>& consensus)
{
    LOCK(csBanAddr_);

    banAddrMempool_.insert(mempool.begin(), mempool.end());

    for (const std::string& addr : consensus)
    {
        size_t pos = addr.find(':');
        if (std::string::npos == pos)
            banAddrConsensus_[addr] = 0;
        else
            banAddrConsensus_[addr.substr(0, pos)] = atoi(addr.substr(pos + 1));
    }
}

bool CContext::MempoolBanActive() const
{
    LOCK(csBanAddr_);
    return !banAddrMempool_.empty();
}

bool CContext::MempoolBanActive(const std::string& addr) const
{
    LOCK(csBanAddr_);
    return banAddrMempool_.find(addr) != banAddrMempool_.end();
}

bool CContext::ConsensusBanActive() const
{
    LOCK(csBanAddr_);
    return !banAddrConsensus_.empty();
}

bool CContext::ConsensusBanActive(
        const std::string& addr,
        int height) const
{
    LOCK(csBanAddr_);
    auto iter = banAddrConsensus_.find(addr);
    if (iter == banAddrConsensus_.end())
        return false;
    else
        return height >= iter->second;
}
