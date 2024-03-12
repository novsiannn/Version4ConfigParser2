/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DataBase.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: novsiann <novsiann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 11:19:53 by nikitos           #+#    #+#             */
/*   Updated: 2024/03/10 14:51:47 by novsiann         ###   ########.fr       */
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
    // if(!this->_variablePath.empty())
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
