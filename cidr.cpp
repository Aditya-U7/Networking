/*

Author: Aditya Upadhye

This is a simple program for printing the subnet mask, broadcast address and network address of a given IPv4 address with CIDR notation.

*/


#include <iostream>
#include <bitset>
#include <vector>
#include <cstdint>


void set_network_and_broadcast_addr(const uint8_t& nwb, const std::vector<std::bitset<8>>& bs, std::vector<std::bitset<8>>& addr, const char which_nw)
{

	uint8_t full_octet = nwb / 8;
	uint8_t partial_octet = nwb % 8 !=0 ? 1: 0;
	uint8_t empty_octet = 4 - (full_octet + partial_octet);

	for (auto& octet: bs)
	{

		if (full_octet >= 1)
		{
			addr.push_back(octet);
			full_octet--;
			continue;
		}

		if (partial_octet)
		{
			uint8_t val = 0;
			uint8_t pow = 128;
			uint8_t br = nwb % 8;
			for (uint8_t i = 0; i < br; i++)
			{
				if (octet[octet.size() - 1 -i] == 1)
					val += pow;
				pow /= 2;
			}
			if (which_nw == 'b')
			{
				uint8_t res = 1;

				for (uint8_t j = (octet.size() - br); j > 0; --j)
				{
					res *= 2;
				}
				val += (res - 1);
			}
			addr.push_back(uint8_t(val));
			partial_octet--;
			continue;
		}


		if (empty_octet >= 1)
		{
			if (which_nw == 'n')
				addr.push_back(uint8_t(0));
			else
				addr.push_back(uint8_t(255));
			empty_octet--;
		}

	}
}


void set_subnet_mask(const uint8_t& nwb, std::vector<std::bitset<8>>& sm)
{

	uint8_t full_octet = nwb / 8;
	uint8_t partial_octet = nwb % 8 !=0 ? 1: 0;
	uint8_t empty_octet = 4 - (full_octet + partial_octet);

	while (full_octet--)
	{
		sm.push_back(uint8_t(255));
	}

	if (partial_octet)
	{
		uint8_t pow = 128;
		uint8_t val = 0;
		for (uint8_t i = nwb % 8; i > 0; --i)
		{
			val += pow;
			pow /= 2;
		}

		sm.push_back(uint8_t(val));

	}

	while (empty_octet--)
	{
		sm.push_back(uint8_t(0));
	}

}


void print_octets_in_binary(const std::vector<std::bitset<8>>& bs, std::string& adr)
{

	uint8_t count = 1;

	for (auto& octet: bs)
	{       
		uint8_t oct_val = 0;
		uint8_t pow = 128;
		for (size_t i = 0; i < octet.size(); ++i)
		{
			std::cout << octet[octet.size() - 1 - i];
			if (octet[octet.size() - 1 - i] == 1)
			{
				oct_val += pow;

			}  
			pow /= 2;
		}
		adr += std::to_string(oct_val);
		if (count < 4)
		{
			std::cout << ".";
			adr += ".";
		}

		count++;
	}

	std::cout << "\n";

}


void check_no_of_network_bits(const uint8_t& nwb)
{

	if (nwb < 0 || nwb > 32)
	{
		std::cout << "Invalid no of network bits.\n";
		exit(1);
	}

}


void check_octet_value(short& val, uint8_t& octet_no)
{


	if (val < 0 || val > 255)
	{

		std::cout << "Resulting in error in octet no: " << +octet_no << "\n";
		exit(1);
	}
	else
	{
		val = 0;
		octet_no++;
	}

}


int main()
{

	std::string ip_addr = "";
	std::cout << "Enter the ip address:\n";
	std::cin >> ip_addr;
	short value = 0;
	uint8_t no_of_host_bits;
	uint8_t no_of_network_bits;
	uint8_t octet_no = 1;
	std::string subnet_mask;
	std::string network_address;
	std::string broadcast_address;
	std::string ipv_addr; 
	std::vector<std::bitset<8>> bset;
	std::vector<std::bitset<8>> bin_subnet_mask;
	std::vector<std::bitset<8>> n_addr;
	std::vector<std::bitset<8>> b_addr;

	for (auto& chr: ip_addr)
	{      

		short tmp;

		if (chr == '/')
		{
			tmp = value;
			check_octet_value(value, octet_no);
			bset.push_back(uint8_t(tmp));
			uint8_t val = 0;
			uint8_t iter_value = ip_addr.length() - 1;

			if (ip_addr[ip_addr.length() - 3] == '/')
			{

				iter_value = (ip_addr.length() - 2);
			}

			while (iter_value != ip_addr.length())
			{

				val = (val * 10) + abs(48 - ip_addr[iter_value]);
				++iter_value;
			}
			no_of_network_bits = val;
			check_no_of_network_bits(no_of_network_bits);
			no_of_host_bits = 32 - no_of_network_bits;
			break;

		}
		if (chr == '.')
		{     
			tmp = value;
			check_octet_value(value, octet_no);
			bset.push_back(uint8_t(tmp));

		}
		else
		{       
			if (chr < 48 || chr > 57)
			{
				std::cout << "\nInvalid character found in octet no: " << +octet_no << "\n";
				exit(1);
			}
			value = (value * 10)  + abs(48 - chr);      

		}

	}


	std::cout << "\n\nNo of network bits: " << +no_of_network_bits << "\n";
	std::cout << "No of host bits: " << +no_of_host_bits << "\n";
	std::cout << "IPv4 in binary: \n";
	print_octets_in_binary(bset, ipv_addr);
	std::cout << "IPv4: " << ipv_addr << "\n";
	set_subnet_mask(no_of_network_bits, bin_subnet_mask);
	std::cout << "Subnet Mask in binary: \n";
	print_octets_in_binary(bin_subnet_mask, subnet_mask);
	std::cout << "Subnet Mask: " << subnet_mask << "\n";
	set_network_and_broadcast_addr(no_of_network_bits, bset, n_addr, 'n');
	set_network_and_broadcast_addr(no_of_network_bits, bset, b_addr, 'b');
	std::cout << "Network Address in Binary:\n"; 
	print_octets_in_binary(n_addr, network_address);
	std::cout << "Network Address: " << network_address << "\n";
	std::cout << "Broadcast Address in Binary:\n"; 
	print_octets_in_binary(b_addr, broadcast_address);
	std::cout << "Broadcast Address: " << broadcast_address << "\n\n";

	return 0;

}
