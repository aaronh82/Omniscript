//
//  Block.cpp
//  OmniScript
//
//  Created by Aaron Houghton on 6/30/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "Block.h"

namespace blocks {

	Block::Block():opcode_(""), next_(nullptr) {
		args_ = {};
		blockArgs_ = {};
		nestedBlocks_ = {};
		listBlocks_ = {};
	}
	
	Block::Block(std::string opcode, block_ptr next): opcode_(opcode), next_(next) {
		args_ = {};
		blockArgs_ = {};
		nestedBlocks_ = {};
		listBlocks_ = {};
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
	
	void Block::addNestedStart(block_ptr b) {
		nestedBlocks_.push_back(b);
	}
	
	std::vector<Block::block_ptr> Block::nestedBlocks() {
		return nestedBlocks_;
	}
	
	void Block::addListBlocks(block_ptr b) {
		listBlocks_.push_back(b);
	}
	
	std::vector<Block::block_ptr> Block::listBlocks() {
		return listBlocks_;
	}
	
	Block::block_ptr Block::next() {
		return next_;
	}
	
	Block::block_ptr Block::next(Block::block_ptr n) {
		next_ = n;
		return next_;
	}
	
}