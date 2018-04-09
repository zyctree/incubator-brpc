// Copyright (c) 2015 Baidu, Inc.
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Authors: wangxuefeng (wangxuefeng@didichuxing.com)
#ifdef ENABLE_THRIFT_FRAMED_PROTOCOL

#include "butil/class_name.h"
#include "brpc/thrift_service.h"
#include "brpc/details/method_status.h"


namespace brpc {

BAIDU_CASSERT(sizeof(thrift_binary_head_t) == 4, sizeof_thrift_must_be_4);

ThriftFramedService::ThriftFramedService() : _additional_space(0) {
    _status = new (std::nothrow) MethodStatus;
    LOG_IF(FATAL, _status == NULL) << "Fail to new MethodStatus";
}

ThriftFramedService::ThriftFramedService(const ThriftFramedServiceOptions& options)
    : _status(NULL), _additional_space(options.additional_space) {
    if (options.generate_status) {
        _status = new (std::nothrow) MethodStatus;
        LOG_IF(FATAL, _status == NULL) << "Fail to new MethodStatus";
    }
}

ThriftFramedService::~ThriftFramedService() {
    delete _status;
    _status = NULL;
}

void ThriftFramedService::Describe(std::ostream &os, const DescribeOptions&) const {
    os << butil::class_name_str(*this);
}

void ThriftFramedService::Expose(const butil::StringPiece& prefix) {
    _cached_name = butil::class_name_str(*this);
    if (_status == NULL) {
        return;
    }
    std::string s;
    s.reserve(prefix.size() + 1 + _cached_name.size());
    s.append(prefix.data(), prefix.size());
    s.push_back('_');
    s.append(_cached_name);
    _status->Expose(s);
}

} // namespace brpc

#endif
