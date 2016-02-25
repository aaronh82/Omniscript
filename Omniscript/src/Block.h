//
//  Block.h
//  OmniScript
//
//  Created by Aaron Houghton on 6/30/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#ifndef __OmniScript__Block__
#define __OmniScript__Block__

#include <iostream>
#include <vector>
#include <string>
#include <memory>

namespace blocks {

	class Block {
		using block_ptr_ = std::shared_ptr<Block>;
		std::string opcode_;
		std::vector<std::string> args_;
		std::vector<block_ptr_> blockArgs_;
		std::vector<block_ptr_> nestedBlocks_;
		std::vector<block_ptr_> listBlocks_;
		block_ptr_ next_;
		
	public:
		using block_ptr = block_ptr_;
		
		Block();
		Block(std::string, block_ptr);
		~Block() {};
		
		void setOpcode(std::string);
		std::string opcode();
		void setArgs(std::vector<std::string>);
		std::vector<std::string> args();
		void setBlockArgs(std::vector<block_ptr>);
		std::vector<block_ptr> blockArgs();
		void addNestedStart(block_ptr);
		std::vector<block_ptr> nestedBlocks();
		void addListBlocks(block_ptr_);
		std::vector<block_ptr> listBlocks();
		block_ptr next();
		block_ptr next(block_ptr);
		
	};

}

#endif /* defined(__OmniScript__Block__) */
