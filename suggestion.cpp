
#include "DataBase.hpp"

// Function to split a string into vector of strings based on delimiter
std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
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

// Function to trim whitespaces
void trimWord(int &start, int &end, std::string line) {
    (void)start;
    end = line.size() - 1;
    while(isspace(line[end]) || line[end] == '{')
        end--;
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

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
        return 1;
    }

    std::string configFile = argv[1];
    std::ifstream file(configFile.c_str());
    if (!file) {
        std::cerr << "Error opening file: " << configFile << std::endl;
        return 1;
    }

    DataBase base;
    std::string line;
    std::string configData;
    std::string lastLine;
    std::map<std::string, int> sectionCounts; // Map to store section counts

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
    // Append the last line if it's not empty
    if (!lastLine.empty()) {
        configData += lastLine;
    }

    std::vector<std::string> lines = split(configData, '\n');
    std::map<std::string, std::vector<std::string> > keyValues;
    int start = 0;
    int end = 0;

    if(checkCurly(configData) )
    {
        std::cerr << "Webserv Error: Quotes are not closed\n";
        return 0;
    }
    std::string currentSection = "";
    for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it) {
        std::string trimmedLine = *it;
        trimmedLine.erase(0, trimmedLine.find_first_not_of(" \t\n\r\f\v"));
        trimmedLine.erase(trimmedLine.find_last_not_of(" \t\n\r\f\v") + 1);

        if (trimmedLine.empty()) {
            continue;  // Skip empty lines
        }
        // Create the section key with count if it's not the first occurrence

        // Check for start of a new section
        if (trimmedLine[trimmedLine.size() - 1] == '{') {
            trimWord(start, end, trimmedLine);
            currentSection = trimmedLine.substr(start, end + 1);
            std::replace(currentSection.begin(), currentSection.end(), ' ', '_');
            base.pushInBase(currentSection);

            // Update section count
            if (sectionCounts.find(currentSection) == sectionCounts.end()) {
                sectionCounts[currentSection] = 0;
            } else {
                sectionCounts[currentSection]++;
            }

            // Create the section key with count if it's not the first occurrence
            if (sectionCounts[currentSection] >= 0) {
                std::stringstream ss;
                ss << "[" << sectionCounts[currentSection] << "]";
                currentSection += ss.str();
            }
        }
        // Check for end of a section
        else if (trimmedLine == "}") {
            base.eraseLastSection();
        }
        // Regular key-value pair
        else {
            std::vector<std::string> tokens = customSplit(trimmedLine, ' ');
            // std::vector<std::string>::iterator tokn_it = tokens.begin();
            // while(tokn_it != tokens.end())
            // {
            //     std::cout << "HERE [" <<  *tokn_it << ']' << std::endl;
            //     tokn_it++;
            // }
            // exit(0);
            if (tokens.size() >= 2) {
                std::string key = tokens[0];
                std::string value = tokens[1];

                // std::cout << "HERE " << key << '-' <<  value << std::endl;
                std::string KeyWithoutLastSection;
                // Combine all remaining tokens as the value
                if (trimmedLine[trimmedLine.size() - 1] == '\''){
                    bool firstLine = true;
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
