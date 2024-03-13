/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikitos <nikitos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 13:10:42 by nikitos           #+#    #+#             */
/*   Updated: 2024/03/13 20:36:15 by nikitos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "DataBase.hpp"

int main(int argc, char *argv[]){

    DataBase base;

    if (argc < 2)
        ft_errors(argv[0],1);

    base.execParser(argv);    
    base.printKeyValue();

    return 0;
}
