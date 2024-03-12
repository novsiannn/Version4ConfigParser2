/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DataBase.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikitos <nikitos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 11:19:53 by nikitos           #+#    #+#             */
/*   Updated: 2024/03/12 22:40:02 by nikitos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DataBase.hpp"

DataBase::DataBase(){}

DataBase::~DataBase(){}

void	DataBase::pushInBase(std::string env_name)
{
    this->_variablePath.push_back(env_name);
}

void	DataBase::eraseLastSection()
{
    if(!this->_variablePath.empty()) // Probably we don't need that condition
        this->_variablePath.pop_back();
}

void	DataBase::printVarPath()
{
	std::vector<std::string>::iterator it;
    for(it = _variablePath.begin(); it != _variablePath.end(); it++){
        
        std::cout << *it << " --> ";
    }
	std::cout << "\n";
}

std::string	DataBase::getFullPathKey()
{
	std::vector<std::string>::iterator it;
	std::string finalKey;

	for(it = _variablePath.begin(); it != _variablePath.end(); it++)
        finalKey += *it + ".";
	return finalKey;
}

std::string DataBase::getKeyWithoutLastSection()
{
    std::vector<std::string>::iterator it;
    std::vector<std::string> tmp = this->_variablePath;
	std::string finalKey;

    if(!tmp.empty())
    {
        tmp.pop_back();
    }
	for(it = tmp.begin(); it != tmp.end(); it++)
        finalKey += *it + ".";
	return finalKey;
}

std::string DataBase::readFile(char **argv)
{
    std::string configData;
    std::string lastLine;
    std::string line;
    std::string configFile = argv[1];
    std::ifstream file(configFile.c_str());

    if (!file) 
        ft_errors(configFile,2);

    while (std::getline(file, line)) {
        std::size_t commentPos = line.find('#');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }
        line.erase(0, line.find_first_not_of(" \t\n\r\f\v")); // Trim leading whitespace
        line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1); // Trim trailing whitespace

        // If the line ends with a space, concatenate with the next line
        if (!line.empty() && line[line.size() - 1] == ' ') {
            lastLine += line.substr(0, line.size() - 1) + " ";
        } else {
            if (!lastLine.empty()) {
                lastLine += line;
                configData += lastLine + "\n";
                lastLine.clear();
            } else {
                configData += line + "\n";
            }
        }
    }
    if (!lastLine.empty()) {
        configData += lastLine;
    }
    if(checkCurly(configData))
        ft_errors("curly ", 3);
    return configData;
}

std::string		DataBase::handleKeySection(int &start, int &end, std::string &line)
{
    std::string currentSection = "";

    trimWordFromEnd(start, end, line);
    currentSection = line.substr(start, end + 1);
    std::replace(currentSection.begin(), currentSection.end(), ' ', '_');
    this->pushInBase(currentSection);
    if (sectionCounts.find(currentSection) == sectionCounts.end()) {
        sectionCounts[currentSection] = 0;
    } else {
        sectionCounts[currentSection]++;
    }
    if (sectionCounts[currentSection] >= 0) {
        std::stringstream ss;
        ss << "[" << sectionCounts[currentSection] << "]";
        currentSection += ss.str();
    }
    return currentSection;
}
