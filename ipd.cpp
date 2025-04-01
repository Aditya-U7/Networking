/*

Author: Aditya Upadhye


This program gives information about IPv4 address' host bits, network bits, network address and broadcast address.
Keep in mind this program works for classful addressing, and when no subnets have been created as that would change the network bits, subnet mask, etc.

Input format: IPv4 with octets separated by a '.'.

*/

#include <iostream>
#include <bitset>
#include <vector>


void print_octets_in_binary(const std::vector<std::bitset<8>>& bs)
{

	short count = 1;
	std::cout << "IPv4 in binary: \n";

	for (auto octet: bs)
	{
		for (size_t i = 0; i < octet.size(); ++i)
		{
			std::cout << octet[octet.size() - 1 - i];

		}
		if (count < 4)
			std::cout << ".";
		count++;

	}
	std::cout << "\n\n";

}


void print_network_and_broadcast_address(const short& nwb, const std::vector<std::bitset<8>>& bs, const char which_addr)
{

	switch(which_addr)
	{
		case 'n':
			std::cout << "The Network Address is: \n";
			break;
		case 'b':
			std::cout << "The Broadcast Address is: \n";
			break;
	}

	short t_nwb = nwb / 8;

	for (auto octet: bs)
	{
		short oct_val = 0;
		short pow = 128;
		for (size_t i = 0; i < octet.size(); ++i)
		{
			if (octet[octet.size() - 1 -i] == 1)
				oct_val += pow;
			pow /= 2;

		}
		std::cout << oct_val << ".";
		t_nwb--;
		if (t_nwb == 0)
			break;
	}



	for (short count = (32 - nwb) /8 ; count > 0; count--)
	{
		switch (which_addr)
		{
			case 'n':
				std::cout << "0";
				if (count != 1)
					std::cout << ".";
				break;
			case 'b':
				std::cout << "255";
				if (count != 1)
					std::cout << ".";
		}
	}
	std::cout << "\n";
}


void check_octet_value(short& val, short& octet_no, short& f_octet)
{


	if (val < 0 || (val > 255 && octet_no > 1) || (val > 223 && octet_no == 1))
	{

		std::cout << "Resulting in error in octet no: " << octet_no << "\n";
		exit(1);
	}
	else
	{

		if (octet_no == 1)
			f_octet = val;

		val = 0;
		if (octet_no < 4)
			octet_no++;

	}

}


int main()
{

	std::string ip_addr = "";
	std::cout << "Enter the ip address:\n";
	std::cin >> ip_addr;
	short value = 0;
	short first_octet;
	std::string ip_class= "No value";
	std::string subnet_mask = "No value"; 
	short no_of_host_bits;
	short no_of_network_bits;
	short octet_no = 1;
	short count = -1;
	std::vector<std::bitset<8>> bset;

	for (auto chr: ip_addr)
	{      
		count++;
		short tmp;

		if (chr == '.')
		{     
			tmp = value;
			check_octet_value(value, octet_no, first_octet);
			bset.push_back(short(tmp));

		}
		else
		{       
			value = (value * 10)  + abs(48 - chr);

			if (count == ip_addr.length()-1)
			{   
				tmp = value;
				check_octet_value(value, octet_no, first_octet);
				bset.push_back(short(tmp));
			}
		}

	}

	if (first_octet >= 0 && first_octet < 128)
	{
		ip_class = "Class A";
		subnet_mask = "255.0.0.0";
		no_of_host_bits = 24; 
		no_of_network_bits = 8; 
	}
	else if (first_octet >= 128 && first_octet < 192)
	{
		ip_class = "Class B";
		subnet_mask = "255.255.0.0"; 
		no_of_host_bits = 16; 
		no_of_network_bits = 16; 
	}
	else 
	{
		ip_class = "Class C";
		subnet_mask = "255.255.255.0"; 
		no_of_host_bits = 8; 
		no_of_network_bits = 24; 
	}

	std::cout << "\n\nClass: " << ip_class << "\n";
	std::cout << "Default Subnet Mask: " << subnet_mask << "\n";
	std::cout << "No of network bits: " << no_of_network_bits << "\n";
	std::cout << "No of host bits: " << no_of_host_bits << "\n";

	print_octets_in_binary(bset);
	print_network_and_broadcast_address(no_of_network_bits, bset, 'n');
	print_network_and_broadcast_address(no_of_network_bits, bset, 'b');

	return 0;

}
