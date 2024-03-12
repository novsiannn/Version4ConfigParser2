/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DataBase.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikitos <nikitos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 11:16:08 by nikitos           #+#    #+#             */
/*   Updated: 2024/03/12 21:16:06 by nikitos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATABASE_H
# define DATABASE_H

#include "AllHeaders.hpp"

class DataBase{
	public: 
		DataBase();
		~DataBase();
		void		pushInBase(std::string env_name);
		void		eraseLastSection();
		void		printVarPath();
		std::string	execParser(char *argv[]);
		std::string	readFile(char **argv);
		std::string	getFullPathKey();
		std::string getKeyWithoutLastSection();
	private:
		std::vector<std::string> _variablePath;
};

#endif
