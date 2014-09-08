//
//  JSON.cpp
//  OmniScript
//
//  Created by Aaron Houghton on 6/30/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "JSON.h"
#include "Logger.h"

#include <exception>

namespace util {
	
	Json::Value JSON::root_;
	block_ptr JSON::startBlock_;
	
	bool JSON::parse(std::istream* is) {
		Json::Reader reader;
		return reader.parse(*is, root_);
	}
	
	std::vector<block_ptr> JSON::build(std::istream *is) {
		if (!parse(is)) {
			LOG(Log::ERROR, "Failed to parse JSON");
			throw "Failed to parse JSON";
		}
		const Json::Value scriptRoot = root_["scripts"];
		std::vector<block_ptr> startingBlocks;
		for (auto& root_iter : scriptRoot) {
			for (auto& script : root_iter) {
				startBlock_ = nullptr;
				
				if (script.isArray()) {
					getInnerBlocks(script);
					startingBlocks.push_back(startBlock_);
				}
			}
		}
		return startingBlocks;
	}
	
	void JSON::getInnerBlocks(Json::Value specs) {
		block_ptr curBlock;
		
		for (auto inner : specs) {
			addBlock(inner, curBlock);
		}
	}
	
	void JSON::addBlock(Json::Value specs, block_ptr &curBlock) {
		block_ptr newBlock(new blocks::Block);
		
		getOpArgs(specs, newBlock);
		
		if (curBlock == nullptr) {
			startBlock_ = newBlock;
			curBlock = startBlock_;
		} else {
			curBlock->next(newBlock);
			curBlock = curBlock->next();
		}
	}
	
//	void JSON::addNestedBlocks(Json::Value specs, block_ptr curBlock) {
//		block_ptr newBlock(new blocks::Block);
//		
//		getOpArgs(specs, newBlock);
//		
//		if (curBlock == nullptr) {
//			curBlock->addNestedArg(newBlock);
//		} else {
//			curBlock->next(newBlock);
//			curBlock = curBlock->next();
//		}
//		for (auto block : curBlock->nestedArgs()) {
//			LOG(Log::DEBUGGING, "Nested block: " + block->opcode());
//		}
//	}
	
	void JSON::getOpArgs(Json::Value specs, block_ptr &curBlock) {
		std::vector<std::string> args;
		std::vector<block_ptr> blockArgs;
		std::string opStr;
		int i(0);
		
		auto opArg = std::begin(specs);
		for (auto end = std::end(specs); opArg != end; ++opArg) {
			if (opArg == specs.begin()) {
				if ((*opArg).isArray()) {
					curBlock->setOpcode("nested");
//					addNestedBlocks(*(*opArg).begin(), b);
//					getOpArgs(*opArg, b);
				} else {
					curBlock->setOpcode((*opArg).asString());
				}
			} else {
				if ((*opArg).isArray()) {
					opStr = "block:" + std::to_string(i++);
					block_ptr b(new blocks::Block);
					getOpArgs(*opArg, b);
					blockArgs.push_back(b);
				} else if ((*opArg).isInt()) {
					opStr = std::to_string((*opArg).asInt());
				} else {
					opStr = (*opArg).asString();
				}
				
				if (!opStr.empty()) {
					args.push_back(opStr);
				} else {
					std::cout << "Error getting arg string.\nExiting..." << std::endl;
					exit(2);
				}
			}
//			LOG(Log::DEBUGGING, "opStr: " + opStr);
//			LOG(Log::DEBUGGING, "curBlock->opcode(): " + curBlock->opcode());
		}
		if (args.size() > 0) curBlock->setArgs(args);
		if (blockArgs.size() > 0) curBlock->setBlockArgs(blockArgs);
	}
	
}