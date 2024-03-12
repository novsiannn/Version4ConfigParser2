/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsFunc.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikitos <nikitos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 13:06:39 by nikitos           #+#    #+#             */
/*   Updated: 2024/03/12 22:15:47 by nikitos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DataBase.hpp"

std::vector<std::string> customSplit(const std::string &s, char delim) {
    std::vector<std::string> port_list;
    std::stringstream iss(s);
    std::string port;
    while (std::getline(iss, port, delim)) {
        if(!port.empty() && port[0] != delim)
            port_list.push_back(port);
    }
    return port_list;
}

std::string getIndexVariableKey(std::string key,std::map<std::string, std::vector<std::string> > keyValues)
{
    typedef std::map<std::string, std::vector<std::string> >::const_iterator MapIterator;
    std::stringstream finalKey;
    MapIterator it;
    int index = 0;

	for(it = keyValues.begin(); it != keyValues.end(); it++)
    {
        if(it->first.substr(0, it->first.size() - 3) == key)
            ++index;
    }
    finalKey << "[" << index << "]";
    return finalKey.str();
}

// Function to trim whitespaces from end
void trimWordFromEnd(int &start, int &end, std::string line) {
    (void)start;
    end = line.size() - 1;
    while(isspace(line[end]) || line[end] == '{')
        end--;
}

void printKeyValue(const std::map<std::string, std::vector<std::string> > &keyValues) {
    typedef std::map<std::string, std::vector<std::string> >::const_iterator MapIterator;
    for (MapIterator it1 = keyValues.begin(); it1 != keyValues.end(); ++it1) {
        std::cout << "Key: " << it1->first
        << "\nValue(s): " << std::endl;
        const std::vector<std::string>& values = it1->second;
        for (std::vector<std::string>::const_iterator it2 = values.begin(); it2 != values.end(); ++it2) {
            std::cout << "  " << *it2 << std::endl;
        }
        std::cout << "\n";
    }
}

// We are not using this function
std::string getValue(const std::map<std::string, std::vector<std::string> > &keyValues, const std::string &key) {
    typedef std::map<std::string, std::vector<std::string> >::const_iterator MapIterator;
    MapIterator it = keyValues.find(key);
    if (it != keyValues.end()) {
        const std::vector<std::string>& values = it->second;
        if (!values.empty()) {
            return values.front(); // Return the first value
        }
    }
    return ""; // Return empty string if key not found or no values
}

// Checks closed curly braces or not
int checkCurly(std::string line) {
    int i = 0;
    int openCurly = 0;
    int closedCurly = 0;

    while(line[i]) {
        if (line[i] == '{')
            ++openCurly;
        else if (line[i] == '}')
            ++closedCurly;
        ++i;
    }
    if (openCurly == closedCurly)
        return 0;
    return 1;
}

void ft_errors(std::string arg, int i)
{
    if(i == 1)
        std::cerr << "Usage: " << arg << " <config_file>" << std::endl;
    if(i == 2)
        std::cerr << "Error opening file: " << arg << std::endl;
    if(i == 3)
        std::cerr << "Quotes are not closed." << std::endl;
    exit(0);
}

std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
