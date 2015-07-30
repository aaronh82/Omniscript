//
//  api.h
//  Omniscript
//
//  Created by Aaron Houghton on 7/29/15.
//  Copyright (c) 2015 CCBAC. All rights reserved.
//

#ifndef Omniscript_api_h
#define Omniscript_api_h

namespace api {
	std::string exec(const std::string& cmd) {
		FILE *pipe = popen(cmd.c_str(), "r");
		if (!pipe) {
			LOG(Log::ERROR, "Failed to execute '" + std::string(cmd) + "'");
		}
		char buffer[128];
		std::string res = "";
		while (!feof(pipe)) {
			if (fgets(buffer, 128, pipe) != NULL) {
				res += buffer;
			}
		}
		pclose(pipe);
		
		if (res.find("YES") != std::string::npos) {
			return "YES";
		}
		return "NO";
	}

}

#endif
