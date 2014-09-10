//
//  Block.cpp
//  OmniScript
//
//  Created by Aaron Houghton on 6/30/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "Block.h"

namespace blocks {

	Block::Block(std::string opcode, block_ptr next): opcode_(opcode), next_(next) {
		nestedStart_ = nullptr;
	}
	
	void Block::setOpcode(std::string op) {
		opcode_ = op;
	}
	
	std::string Block::opcode() {
		return opcode_;
	}
	
	void Block::setArgs(std::vector<std::string> args) {
		args_ = args;
	}
	
	std::vector<std::string> Block::args() {
		return args_;
	}
	
	void Block::setBlockArgs(std::vector<block_ptr> blockArgs) {
		blockArgs_ = blockArgs;
	}
	
	std::vector<Block::block_ptr> Block::blockArgs() {
		return blockArgs_;
	}
	
	void Block::setNestedStart(block_ptr b) {
		nestedStart_ = b;
	}
	
	Block::block_ptr Block::nestedStart() {
		return nestedStart_;
	}
	
	Block::block_ptr Block::next(Block::block_ptr n) {
		if (n) {
			next_ = n;
		}
		return next_;
	}
	
}