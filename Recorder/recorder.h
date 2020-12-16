#pragma once
#define _UNICODE
#define UNICODE

#include "records.pb.h"

#include <thread>
#include <string>
#include <vector>
#include <iostream>


#include <boost/program_options.hpp>
#include <boost/process.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>

#include <Windows.h>