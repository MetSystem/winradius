#pragma once

class value_pair
{
public:
	value_pair(void);
	~value_pair(void);
	uint8_t id;
	uint8_t *data;
	void encode();
	void decode(uint8_t *data);
};
