//
//  Operators.cpp
//  Omniscript
//
//  Created by Aaron Houghton on 9/2/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "Operators.h"
#include "Interpreter.h"
#include "Variables.h"

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <numeric>

namespace interp {
	
	float add::operator()(const block_ptr &b, Interpreter &interp) {
		float x = (b->args()[0].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
					std::stof(b->args()[0]);
		float y = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					std::stof(b->args()[1]);
		
		LOG(Log::DEBUGGING, "add");
		return x + y;
	}
	
	float subtract::operator()(const block_ptr &b, Interpreter &interp) {
		float x = (b->args()[0].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
					std::stof(b->args()[0]);
		float y = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					std::stof(b->args()[1]);
		
		LOG(Log::DEBUGGING, "subtract");
		return x - y;
	}
	
	float multiply::operator()(const block_ptr &b, Interpreter &interp) {
		float x = (b->args()[0].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
					std::stof(b->args()[0]);
		float y = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					std::stof(b->args()[1]);
		
		LOG(Log::DEBUGGING, "multiply");
		return x * y;
	}
	
	float divide::operator()(const block_ptr &b, Interpreter &interp) {
		float x = (b->args()[0].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
					std::stof(b->args()[0]);
		float y = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					std::stof(b->args()[1]);
		
		LOG(Log::DEBUGGING, "divide");
		return x/y;
	}
	
	int randomFromTo::operator()(const block_ptr &b, Interpreter &interp) {
		int x = (b->args()[0].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
					std::stoi(b->args()[0]);
		int y = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					std::stoi(b->args()[1]);
		
		std::srand((unsigned)time(NULL));
		
		LOG(Log::DEBUGGING, "randomFromTo");
		return (std::rand() % (y - x + 1)) + x;
	}
	
	bool lessThan::operator()(const block_ptr &b, Interpreter &interp) {
		float x = (b->args()[0].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
					std::stof(b->args()[0]);
		float y = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					std::stof(b->args()[1]);
		
		LOG(Log::DEBUGGING, "lessThan");
		return x < y;
	}
	
	bool equalTo::operator()(const block_ptr &b, Interpreter &interp) {
		float x = (b->args()[0].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
					std::stof(b->args()[0]);
		float y = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					std::stof(b->args()[1]);
		
		LOG(Log::DEBUGGING, "equalTo");
		return x == y;
	}
	
	bool greaterThan::operator()(const block_ptr &b, Interpreter &interp) {
		float x = (b->args()[0].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
					std::stof(b->args()[0]);
		float y = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					std::stof(b->args()[1]);
		
		LOG(Log::DEBUGGING, "greaterThan");
		return x > y;
	}
	
	bool logicalAnd::operator()(const block_ptr &b, Interpreter &interp) {
		bool x = (b->args()[0].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
					b->args()[0] == "true" ? true : false;
		bool y = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					b->args()[1] == "true" ? true : false;
		
		LOG(Log::DEBUGGING, "logicalAnd");
		return x && y;
	}
	
	bool logicalOr::operator()(const block_ptr &b, Interpreter &interp) {
		bool x = (b->args()[0].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
					b->args()[0] == "true" ? true : false;
		bool y = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					b->args()[1] == "true" ? true : false;
		
		LOG(Log::DEBUGGING, "logicalOr");
		return x || y;
	}
	
	bool logicalNegation::operator()(const block_ptr &b, Interpreter &interp) {
		bool x = (b->args()[0].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
					b->args()[0] == "true" ? true : false;
		
		LOG(Log::DEBUGGING, "logicalNegation");
		return !x;
	}
	
	int modulo::operator()(const block_ptr &b, Interpreter &interp) {		
		int x = (b->args()[0].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
					std::stoi(b->args()[0]);
		int y = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					std::stoi(b->args()[1]);
		
		LOG(Log::DEBUGGING, "modulo");
		return x % y;
	}
	
	float roundToNearest::operator()(const block_ptr &b, Interpreter &interp) {
		float x = (b->args()[0].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
					std::stof(b->args()[0]);
		
		LOG(Log::DEBUGGING, "round");
		return std::round(x);
	}
	
	float computeFunction::operator()(const block_ptr &b, Interpreter &interp) {
		std::string func = b->args()[0];
		float x = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					std::stof(b->args()[1]);
		
		if (func.compare("abs") == 0) {
			return std::abs(x);
		} else if (func.compare("floor") == 0) {
			return std::floor(x);
		} else if (func.compare("ceiling") == 0) {
			return std::ceil(x);
		} else if (func.compare("sqrt") == 0) {
			return std::sqrt(x);
		} else if (func.compare("sin") == 0) {
			return std::sin(x);
		} else if (func.compare("cos") == 0) {
			return std::cos(x);
		} else if (func.compare("tan") == 0) {
			return std::tan(x);
		} else if (func.compare("asin") == 0) {
			return std::asin(x);
		} else if (func.compare("acos") == 0) {
			return std::acos(x);
		} else if (func.compare("atan") == 0) {
			return std::atan(x);
		} else if (func.compare("ln") == 0) {
			return std::log(x);
		} else if (func.compare("log") == 0) {
			return std::log10(x);
		} else if (func.compare("e^") == 0) {
			return std::exp(x);
		} else if (func.compare("10^") == 0) {
			return std::pow(10, x);
		}
		
		LOG(Log::DEBUGGING, "computeFunction");
		return -1;
	}
	
	void maxOf::operator()(const block_ptr &b, Interpreter &interp) {
		std::vector<block_ptr>::iterator maxBlock = std::max_element(b->nestedBlocks().begin(), b->nestedBlocks().end());
		const float maxValue = interp.execute(*maxBlock);
		if (b->blockArgs()[0]->opcode().compare("readVariable") == 0) {
			(*std::find_if(interp.variables().begin(), interp.variables().end(), findVariable(b->blockArgs()[0]->args()[0])))->value(maxValue);
		} else if (b->blockArgs()[0]->opcode().compare("readPoint") == 0) {
			writePointValue(maxValue, std::find_if(interp.points().begin(), interp.points().end(), findVariable(b->blockArgs()[0]->args()[0])));
		}
	}
	
	void minOf::operator()(const block_ptr &b, Interpreter &interp) {
		std::vector<block_ptr>::iterator minBlock = std::min_element(b->nestedBlocks().begin(), b->nestedBlocks().end());
		const float minValue = interp.execute(*minBlock);
		if (b->blockArgs()[0]->opcode().compare("readVariable") == 0) {
			(*std::find_if(interp.variables().begin(), interp.variables().end(), findVariable(b->blockArgs()[0]->args()[0])))->value(minValue);
		} else if (b->blockArgs()[0]->opcode().compare("readPoint") == 0) {
			writePointValue(minValue, std::find_if(interp.points().begin(), interp.points().end(), findVariable(b->blockArgs()[0]->args()[0])));
		}
	}
	
	void avg::operator()(const block_ptr &b, Interpreter &interp) {
		auto getVal = [&](float x, const block_ptr &block) { return x += interp.execute(block); };
		const float average = std::accumulate(b->nestedBlocks().begin(), b->nestedBlocks().end(), 0, getVal) / b->nestedBlocks().size();
		if (b->blockArgs()[0]->opcode().compare("readVariable") == 0) {
			(*std::find_if(interp.variables().begin(), interp.variables().end(), findVariable(b->blockArgs()[0]->args()[0])))->value(average);
		} else if (b->blockArgs()[0]->opcode().compare("readPoint") == 0) {
			writePointValue(average, std::find_if(interp.points().begin(), interp.points().end(), findVariable(b->blockArgs()[0]->args()[0])));
		}
	}
	
	float getValueOf::operator()(const block_ptr &b, Interpreter &interp) {
		return (b->args()[0].find("block:") != std::string::npos) ?
				interp.execute(b->blockArgs()[std::stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
				std::stof(b->args()[0]);
	}
}