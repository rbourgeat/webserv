#include "webserv.hpp"

struct location	addLocation(std::string serverBlock, size_t *i)
{
	struct location loc;

	loc.redi.num = -1;
	(*i)+= 8;
	while (serverBlock[*i] == ' ' || serverBlock[*i] == '\t')
		(*i)++;
	while (serverBlock[*i] != ' ' && serverBlock[*i] != '\t')
	{
		loc.path.push_back(serverBlock[*i]);
		(*i)++;
	}
	while (serverBlock[*i] != '\n')
		(*i)++;
	(*i)++;
	while (serverBlock[*i] && serverBlock[*i] != '}')
	{   
		if ((strcmp(serverBlock.substr(*i, 4).c_str(), "root")) == 0)
		{
			(*i)+= 4;
			int j = 0;
			while (serverBlock[*i] && (serverBlock[*i] == ' ' || serverBlock[*i] == '\t'))
				(*i)++;
			while (serverBlock[*i] && serverBlock[*i] != ';')
			{
				(*i)++;
				j++;
			}
			loc.root = serverBlock.substr(*i - j, j);
			while (serverBlock[*i] && serverBlock[*i] != '\n')
				(*i)++;
		}
		if ((strcmp(serverBlock.substr(*i, 6).c_str(), "method")) == 0)
		{
			(*i)+= 6;
			int j = 0;
			while (serverBlock[*i] && (serverBlock[*i] == ' ' || serverBlock[*i] == '\t'))
				(*i)++;
			while (serverBlock[*i] && serverBlock[*i] != ';')
			{
				while (serverBlock[*i] && serverBlock[*i] != ' ' && serverBlock[*i] != ';')
				{
					(*i)++;
					j++;
				}
				loc.method.push_back(serverBlock.substr(*i - j, j));
				j = 0;
				if (serverBlock[*i] == ' ')
					(*i)++;
			}
			while (serverBlock[*i] && serverBlock[*i] != '\n')
				(*i)++;
		}
		if ((strcmp(serverBlock.substr(*i, 6).c_str(), "return")) == 0)
		{
			struct redirection redirect;
			redirect.num = 0;
			int j = 0;
			(*i)+= 6;
			while (serverBlock[*i] && (serverBlock[*i] == ' ' || serverBlock[*i] == '\t'))
				(*i)++;
			while (serverBlock[*i] && serverBlock[*i] >= '0' && serverBlock[*i] <= '9')
			{
				(*i)++;
				j++;
			}
			redirect.num = atoi(serverBlock.substr(*i - j, j).c_str());
			while (serverBlock[*i] && (serverBlock[*i] == ' ' || serverBlock[*i] == '\t'))
				(*i)++;
			while (serverBlock[*i] && serverBlock[*i] != ';')
			{
				redirect.path += serverBlock[*i];
				(*i)++;
			}
			loc.redi = redirect;
			while (serverBlock[*i] && serverBlock[*i] != '\n')
				(*i)++;
		}
		if ((strcmp(serverBlock.substr(*i, 7).c_str(), "default")) == 0)
		{
			int j = 0;
			(*i)+= 7;
			while (serverBlock[*i] && (serverBlock[*i] == ' ' || serverBlock[*i] == '\t'))
				(*i)++;
			while (serverBlock[*i] && serverBlock[*i] != ';')
			{
				(*i)++;
				j++;
			}
			loc.defaultFile = serverBlock.substr(*i - j, j);
			while (serverBlock[*i] && serverBlock[*i] != '\n')
				(*i)++;
		}
		if ((strcmp(serverBlock.substr(*i, 6).c_str(), "upload")) == 0)
		{
			int j = 0;
			(*i)+= 6;
			while (serverBlock[*i] && (serverBlock[*i] == ' ' || serverBlock[*i] == '\t'))
				(*i)++;
			while (serverBlock[*i] && serverBlock[*i] != ';')
			{
				(*i)++;
				j++;
			}
			loc.uploadDir = serverBlock.substr(*i - j, j);
			while (serverBlock[*i] && serverBlock[*i] != '\n')
				(*i)++;
		}
		if ((strcmp(serverBlock.substr(*i, 7).c_str(), "dirList")) == 0)
		{
			int j = 0;
			(*i)+= 7;
			while (serverBlock[*i] && (serverBlock[*i] == ' ' || serverBlock[*i] == '\t'))
				(*i)++;
			while (serverBlock[*i] && serverBlock[*i] != ';')
			{
				(*i)++;
				j++;
			}
			loc.dirList = serverBlock.substr(*i - j, j);
			while (serverBlock[*i] && serverBlock[*i] != '\n')
				(*i)++;
		}
		(*i)++;
	}
	(*i)++;
	return (loc);
}

void	addServer(std::vector<struct server> *servers, std::string serverBlock)
{
	size_t	i(0);
	size_t	j(0);
	struct server	s;

	s.max_body_size = 0;
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
		if ((strcmp(serverBlock.substr(i, 10).c_str(), "error_page")) == 0)
		{
			struct errorPage eP;
			eP.num = 0;
			int j = 0;
			i+= 10;
			while (serverBlock[i] && (serverBlock[i] == ' ' || serverBlock[i] == '\t'))
				i++;
			while (serverBlock[i] && serverBlock[i] >= '0' && serverBlock[i] <= '9')
			{
				i++;
				j++;
			}
			eP.num = atoi(serverBlock.substr(i - j, j).c_str());
			while (serverBlock[i] && (serverBlock[i] == ' ' || serverBlock[i] == '\t'))
				i++;
			while (serverBlock[i] && serverBlock[i] != ';')
			{
				eP.path += serverBlock[i];
				i++;
			}
			s.error.push_back(eP);
			while (serverBlock[i] && serverBlock[i] != '\n')
				i++;
		}
		if ((strcmp(serverBlock.substr(i, 20).c_str(), "client_max_body_size")) == 0)
		{
			i+= 20;
			int j = 0;
			while (serverBlock[i] && (serverBlock[i] == ' ' || serverBlock[i] == '\t'))
				i++;
			while (serverBlock[i] && serverBlock[i] != ';')
			{
				i++;
				j++;
			}
			s.max_body_size = atoi(serverBlock.substr(i - j, j).c_str());
			while (serverBlock[i] && serverBlock[i] != '\n')
				i++;
		}
		if ((strcmp(serverBlock.substr(i, 3).c_str(), "cgi")) == 0)
        {
            int j = 0;
            i+= 3;
            while (serverBlock[i] && (serverBlock[i] == ' ' || serverBlock[i] == '\t'))
                i++;
            while (serverBlock[i] && serverBlock[i] != ';' && serverBlock[i] != '\n')
            {
                while (serverBlock[i] && serverBlock[i] != ' ' && serverBlock[i] != ';')
                {
                    i++;
                    j++;
                }
                s.extensions.push_back(serverBlock.substr(i - j, j));
				j = 0;
				i++;
            }
            while (serverBlock[i] && serverBlock[i] != '\n')
                i++;
        }
		if ((strcmp(serverBlock.substr(i, 8).c_str(), "location")) == 0)
			s.loc.push_back(addLocation(serverBlock, &i));
		if (serverBlock[i] == '#')
			while (serverBlock[i] != '\n')
				i++;
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
						if (line[i] == '}' && line[i + 1] == ';')
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
		for (size_t j(0); j < (*servers)[i].error.size(); j++)
		{
			std::cout << "error num: " << (*servers)[i].error[j].num << std::endl;
			std::cout << "error path: " << (*servers)[i].error[j].path << std::endl;
		}
		for (size_t j(0); j < (*servers)[i].loc.size(); j++)
		{
			std::cout << "Location [" << (*servers)[i].loc[j].path << "]" << std::endl;
			std::cout << "-->root: " << (*servers)[i].loc[j].root << std::endl;
			std::cout << "-->method: ";
			for (size_t k(0); k < (*servers)[i].loc[j].method.size(); k++)
				std::cout << (*servers)[i].loc[j].method[k] << " ";
			std::cout << std::endl;
			std::cout << "-->redirection num: " << (*servers)[i].loc[j].redi.num << ", redirection path: " << (*servers)[i].loc[j].redi.path << std::endl;
			std::cout << "-->defaultFile: " << (*servers)[i].loc[j].defaultFile << std::endl;
			std::cout << "-->uploadDir: " << (*servers)[i].loc[j].uploadDir << std::endl;
			std::cout << "-->dirList: " << (*servers)[i].loc[j].dirList << std::endl;
		}
		std::cout << "client_max_body_size: " << (*servers)[i].max_body_size << std::endl;
        std::cout << "extensions: ";
        for (size_t k(0); k < (*servers)[i].extensions.size(); k++)
            std::cout << (*servers)[i].extensions[k] << " ";
		std::cout << std::endl;
		std::cout << std::endl;
	}
	//close file?
}
