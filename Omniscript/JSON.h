//
//  JSON.h
//  OmniScript
//
//  Created by Aaron Houghton on 6/30/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#ifndef __OmniScript__JSON__
#define __OmniScript__JSON__

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "jsoncpp/json/json.h"
#include "Block.h"

using block_ptr = blocks::Block::block_ptr;

namespace util {

	class JSON {
		static Json::Value root_;
		static block_ptr startBlock_;
		
		JSON() {}
		
		static bool parse(std::istream*);
		static void getInnerBlocks(Json::Value);
		static void addBlock(Json::Value, block_ptr&);
		static void addNestedBlocks(Json::Value, block_ptr&);
		static void getOpArgs(Json::Value, block_ptr&);
		
	public:
		static std::vector<block_ptr> build(std::istream*);
		
	};
	
}


#endif /* defined(__OmniScript__JSON__) */
