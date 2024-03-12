/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikitos <nikitos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 13:10:42 by nikitos           #+#    #+#             */
/*   Updated: 2024/03/12 22:40:46 by nikitos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "DataBase.hpp"

std::string   DataBase::execParser(char *argv[])
{
    std::string configData;
    configData = this->readFile(argv);
    return configData;
}

int main(int argc, char *argv[]) {

    DataBase base;

    if (argc < 2)
        ft_errors(argv[0],1);
    
    std::string configData;
    configData = base.execParser(argv);

    std::map<std::string, int> sectionCounts;

    std::vector<std::string> lines = customSplit(configData, '\n');
    std::map<std::string, std::vector<std::string> > keyValues;
    int start = 0;
    int end = 0;


    std::string currentSection = "";
    for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it) {
        std::string trimmedLine = *it;
        trimmedLine.erase(0, trimmedLine.find_first_not_of(" \t\n\r\f\v"));
        trimmedLine.erase(trimmedLine.find_last_not_of(" \t\n\r\f\v") + 1);

        if (trimmedLine.empty()) {
            continue; 
        }

        if (trimmedLine[trimmedLine.size() - 1] == '{') {
            currentSection = base.handleKeySection(start, end, trimmedLine);
        }
        else if (trimmedLine[trimmedLine.size() - 1] == '}') {
            base.eraseLastSection();
        }
        else {
            std::vector<std::string> tokens = customSplit(trimmedLine, ' ');
            if (tokens.size() >= 2) {
                std::string key = tokens[0];
                std::string value = tokens[1];

                // std::cout << "HERE " << key << '-' <<  value << std::endl;
                std::string KeyWithoutLastSection;
                // Combine all remaining tokens as the value
                if (trimmedLine[trimmedLine.size() - 1] == '\''){
                    bool firstLine = true;
                    value = "";
                    while (true)
                    {
                        trimmedLine = *it;
                        tokens = split(trimmedLine, ' ');
                        std::vector<std::string>::iterator token_it = tokens.begin();
                        while (token_it != tokens.end())
                        {
                            if (firstLine)
                            {
                                value += *(++token_it) + " ";
                                firstLine = false;
                            }
                            else
                                value += *token_it++ + " ";
                        }
                        it++;
                        if (trimmedLine[trimmedLine.size() - 1] == ';' || trimmedLine[trimmedLine.size() - 1] == '}')
                            break ;
                    }
                }
                else{
                    for (size_t i = 2; i < tokens.size(); ++i) {
                        value += " " + tokens[i];
                    }
                }
                KeyWithoutLastSection = base.getKeyWithoutLastSection();
                std::vector<std::string> splitedValue = customSplit(value, ' ');
                std::vector<std::string>::iterator spltValIt = splitedValue.begin();
                while (spltValIt != splitedValue.end())
                {
                    if (!currentSection.empty()) {
                        keyValues[KeyWithoutLastSection + currentSection + "." + key].push_back(*spltValIt);
                    } else {
                        std::string indexKey = getIndexVariableKey(key, keyValues);
                        while (spltValIt != splitedValue.end())
                        {
                            keyValues[key + indexKey].push_back(*spltValIt); // For keys not inside a section
                            spltValIt++;
                        }
                        break;
                    }
                    spltValIt++;
                }
                // base.printVarPath();
            }
        }
    }

    // Print the result
    printKeyValue(keyValues);

    // Get value by key example
    // std::cout << "Value for 'worker_processes': " << getValue(keyValues, "worker_processes") << std::endl;

    return 0;
}
