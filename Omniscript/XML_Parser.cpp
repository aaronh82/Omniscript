//
//  XML_Parser.cpp
//  Omniscript
//
//  Created by Aaron Houghton on 12/12/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "XML_Parser.h"

#include <sstream>
#include <string>
#include <utility>
#include <cstring>
#include <cstdlib>

namespace util {
	
	node_ptr XML::root;
	rapidxml::xml_document<> XML::doc;
	std::string XML::content;
	
	void XML::parse(std::istream *is) {
		std::stringstream buf;
		buf << is->rdbuf();
		content = buf.str();
		doc.parse<0>(&content[0]);
		root.reset(doc.first_node()->first_node("stage")->first_node("sprites")
				   ->first_node("sprite"), null_deleter());
	}
	
	std::map<int, block_ptr> XML::build(std::istream *is, script::Script &script) {
		parse(is);
		node_ptr scriptRoot(root->first_node("scripts")->first_node("script"), null_deleter());
		getVars(script.variables_);
		getPoints(script.points_);
		std::map<int, block_ptr> startingBlocks;
		
		for (int i = 0; scriptRoot; scriptRoot.reset(scriptRoot->next_sibling(), null_deleter()), ++i) {
			block_ptr b(getChildren(node_ptr(scriptRoot->first_node(), null_deleter())));
			startingBlocks.insert(std::pair<int, block_ptr>(i, b));
		}
		return startingBlocks;
	}
	
	block_ptr XML::getChildren(const node_ptr spec, bool nested) {
		block_ptr startBlock, curBlock;
		node_ptr tmpSpec(spec);
		
		while (tmpSpec) {
			block_ptr newBlock(new blocks::Block);
			
			getOpArgs(tmpSpec, newBlock);
			
			if (curBlock == nullptr) {
				startBlock = newBlock;
				curBlock = startBlock;
			} else {
				curBlock->next(newBlock);
				curBlock = curBlock->next();
			}
			
			tmpSpec.reset(tmpSpec->next_sibling(), null_deleter());
		}
		
		if (!nested) {
			curBlock->next(startBlock);
		}
		
		return startBlock;
	}
	
	void XML::getOpArgs(const node_ptr script, block_ptr curBlock) {
		std::vector<std::string> args;
		std::vector<block_ptr> blockArgs;
		std::string opStr;
		int i(0);
		
		node_ptr tmpScript(script);
		
		if (std::strcmp(tmpScript->name(), "block") == 0 ||
			std::strcmp(tmpScript->name(), "list") == 0) {
			if (tmpScript->first_attribute("s")) {
				curBlock->setOpcode(tmpScript->first_attribute("s")->value());
			} else if (tmpScript->first_attribute("var")) {
				curBlock->setOpcode("readVariable");
				opStr = tmpScript->first_attribute("var")->value();
			} else if (tmpScript->first_attribute("point")) {
				curBlock->setOpcode("readPoint");
				opStr = tmpScript->first_attribute("point")->value();
			} else if (tmpScript->first_attribute("schedule")) {
				curBlock->setOpcode("reportGetSched");
				opStr = tmpScript->first_attribute("schedule")->value();
			}
			
			if (!opStr.empty()) {
				args.push_back(opStr);
			}
			
			const node_ptr first(tmpScript->first_node(), null_deleter());
			for (node_ptr child(first);
				 child;
				 child.reset(child->next_sibling(), null_deleter())) {
				if (std::strcmp(child->name(), "block") == 0 ||
					std::strcmp(child->name(), "script") == 0 ||
					std::strcmp(child->name(), "list") == 0) {
					block_ptr b(new blocks::Block);
					getOpArgs(child, b);
					if (b->opcode() == "nested") {
						curBlock->addNestedStart(b->nestedBlocks()[0]);
					} else if (b->opcode().empty()) {
						blockArgs = b->blockArgs();
					} else {
						opStr = "block:" + std::to_string(i++);
						blockArgs.push_back(b);
					}
				} else if (std::strcmp(child->name(), "l") == 0) {
					if (child->first_node() != 0) {
						opStr = child->value();
					} else {
						opStr = child->first_node("option")->value();
					}
				}
				
				if (!opStr.empty()) {
					args.push_back(opStr);
				}
			}
		} else if (std::strcmp(tmpScript->name(), "script") == 0) {
			curBlock->setOpcode("nested");
			node_ptr child(tmpScript->first_node(), null_deleter());
			curBlock->addNestedStart(getChildren(child, true));
		}
		
		if (args.size() > 0) curBlock->setArgs(args);
		if (blockArgs.size() > 0) curBlock->setBlockArgs(blockArgs);
	}
	
	void XML::getVars(std::vector<std::shared_ptr<interp::Variable> > &variables) {
		const rapidxml::xml_node<> *varRoot(root->first_node("variables"));
		if (varRoot->first_node()) {
			for (rapidxml::xml_node<> *var(varRoot->first_node());
				 var;
				 var = var->next_sibling()) {
				variables.emplace_back(std::make_shared<interp::Variable>(
										var->first_attribute("name")->value(),
										var->first_attribute("type")->value(),
										std::atof(var->first_node("l")->value())));
			}
		}
	}
	
	void XML::getPoints(std::vector<std::shared_ptr<interp::Point> > &points) {
		const rapidxml::xml_node<> *devices(root->first_node("devices"));
		if (devices) {
			for (rapidxml::xml_node<> *device(devices->first_node());
				 device;
				 device = device->next_sibling()) {
				unsigned int id = std::atof(device->first_attribute("device_id")->value());
				const std::string name = device->first_attribute("name")->value();
				unsigned int path = std::atof(device->first_attribute("path_id")->value());
				const rapidxml::xml_node<> *pointRoot(device->first_node("points"));
				for (rapidxml::xml_node<> *point(pointRoot->first_node());
					 point;
					 point = point->next_sibling()) {
					points.emplace_back(std::make_shared<interp::Point>(
									point->first_attribute("name")->value(),
									point->first_attribute("type")->value(),
									std::atof(point->first_node("l")->value()),
									std::atof(point->first_attribute("type_id")->value()),
									id,
									name,
									path));
				}
			}
		}
	}
}
