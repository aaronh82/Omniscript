//
//  XML_Parser.h
//  Omniscript
//
//  Created by Aaron Houghton on 12/12/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#ifndef __Omniscript__XML_Parser__
#define __Omniscript__XML_Parser__

#include "rapidxml.hpp"
#include "Block.h"
#include "Interpreter.h"

#include <iostream>
#include <vector>
#include <map>
#include <memory>

using block_ptr = blocks::Block::block_ptr;
using node_ptr = std::shared_ptr<rapidxml::xml_node<> >;

namespace script {
	class Script;
}

namespace util {
	
	class null_deleter {
	public:
		void operator()(void const *) const {}
	};
	
	class XML {
		
		static std::string content;
		static rapidxml::xml_document<> doc;
		static node_ptr root;
		
		XML() {}
		
		static void parse(std::istream*);
		static block_ptr getChildren(const node_ptr, bool = false);
		static void addBlock(node_ptr, block_ptr&);
		static void getOpArgs(const node_ptr, block_ptr);
		
	public:
		static std::map<int, block_ptr> build(std::istream*, script::Script&);
		static void getVars(std::vector<std::shared_ptr<interp::Variable> >&);
		static void getPoints(std::vector<std::shared_ptr<interp::Point> >&,
							  std::vector<std::shared_ptr<interp::Device> >&);
	};
}

#endif /* defined(__Omniscript__XML_Parser__) */
