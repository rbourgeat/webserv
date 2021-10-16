#include "webserv.hpp"

void	addServer(std::vector<struct server> *servers, std::string serverBlock)
{
		size_t	i(0);
		size_t	j(0);
		struct server	s;

		while (serverBlock[i])
		{
				if ((strcmp(serverBlock.substr(i, 6).c_str(), "listen")) == 0)
				{
					i+= 6;
					while (serverBlock[i] && (serverBlock[i] == ' ' || serverBlock[i] == '\t'))
						i++;
					j+= i;
					while (serverBlock[j] && serverBlock[j] >= '0' && serverBlock[j] <= '9')
						j++;
					std::istringstream iss(serverBlock.substr(i, j - i).c_str());
					iss >> s.port;
					j = 0;
					//std::cout << "port: " << s.port << std::endl;
					while (serverBlock[i] && serverBlock[i] != '\n')
						i++;
				}
				if ((strcmp(serverBlock.substr(i, 11).c_str(), "server_name")) == 0)
				{
					i+= 11;
					while (serverBlock[i] && (serverBlock[i] == ' ' || serverBlock[i] == '\t'))
            i++;
          j+= i;
          while (serverBlock[j] && serverBlock[j] != ';')
            j++;
          s.name = serverBlock.substr(i, j - i);
          j = 0;
          //std::cout << "server_name: " << s.name << std::endl;
        	while (serverBlock[i] && serverBlock[i] != '\n')
            i++;
				}
				if (serverBlock[i] == '\n')
					i++;
		}
		(*servers).push_back(s);
}

void	readConfig(char const *argv, std::vector<struct server> *servers)
{
		std::ifstream config(argv);
		std::string		line;
		std::string		serverBlock;
		size_t						i(0);
		int						bracket(0);

		if (config)
		{
			while (getline(config, line))
				{
						while (line[i] && (line[i] == ' ' || line[i] == '\t'))
							i++;
						if (line[i] && ((i + 5) < line.size()))
						{
							if ((strcmp(line.substr(i, 6).c_str(), "server")) == 0)
							{
								while (getline(config, line) && bracket == 0)
								{
										i = 0;
										while (line[i] && (line[i] == ' ' || line[i] == '\t'))
											i++;
										if (line[i] == '}')
											bracket = 1;
										else
												serverBlock += line.substr(i) + '\n';
								}
								bracket = 0;
								//std::cout << "server block:\n" << serverBlock << std::endl;
								addServer(servers, serverBlock);
								serverBlock.erase();
							}
						}
						i = 0;
				}

		}
		else
		{
			std::cerr << "ERROR: while opening config file" << std::endl;
			exit(EXIT_FAILURE);
		}
		for (size_t i(0); i < (*servers).size(); i++)
		{
			std::cout << "name: " << (*servers)[i].name << std::endl;
			std::cout << "port: " << (*servers)[i].port << std::endl;
			std::cout << std::endl;
		}
		//close file?
}
