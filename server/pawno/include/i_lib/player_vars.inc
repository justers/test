new reg_skins [ 4 ] [ 2 ] ={
	{79,77},
	{2,93},
	{7,151},
	{78,198}
};
enum
{
	d_none,
	d_profile_password,
	d_profile_name,
	d_list_character,
	d_create_character,
	d_character_age,
	d_character_gender,
	d_character_name,
	d_give_object_id,
	d_give_object_amount,
	d_sell_object_price,
	d_accept_object_buy
}

enum p_s_e_info
{
	bool:p_logged,
	bool:p_dialog,
	p_ip [ 32 ],
}
new p_s_info [ MAX_PLAYERS ][ p_s_e_info ] ;

enum p_e_info
{
	name [ MAX_PLAYER_NAME ],
	id,
	main_timer,
	speed_timer,
	level,
	exp,
	age,
	gender,
	skin,
	money,
	inventory_slot [ 10 ],
	inventory_amount [ 10 ],
	bool:license [ 4 ],
}
new p_info [ MAX_PLAYERS ][ p_e_info ] ;

enum profile_e_info
{
	profile_increment,
	profile_name [ MAX_PLAYER_NAME ],
}
new profile_info [ MAX_PLAYERS ][ profile_e_info ] ;


enum item_info_enum 
{ 
    item_id,
    item_name [ 64 ], 
    Float:rot_x, 
    Float:rot_y, 
    Float:rot_z, 
    Float:zoom 
}; 
new item_info [ 2 ] [ item_info_enum ] = { 
    {1649, " ", 0.000000, 0.000000, 90.000000, 2.000000}, 
    {2223, "Поднос с едой", -16.000000, 0.000000, -55.000000, 1.000000} 
};

new clicked_item [ MAX_PLAYERS ] ;

enum drop_enum
{
    d_id,
    d_model,
    Float:d_pos [ 3 ],
    d_virtual_world,
    d_interior,
    Text3D:d_label,
	d_object
};
new drop_obj_info [ MAX_PLAYERS ] [ drop_enum ] ;


enum _vehicle
{
//===============[личные авто]===============
	v_id,
	v_vehicle,
	v_owner,
	v_model,
	v_color [ 2 ],
	Float:v_pos [ 4 ],
	Float:v_now_pos [ 4 ],
	Float:v_millage,
	Float:v_fuel,
	v_plate [ 24 ],
	bool:v_locked,
//===============[]===============
}

new veh_info [ MAX_VEHICLES ] [ _vehicle ];

enum _areas
{
	a_id,
	a_area [ 2 ],
	a_type,
	Float:a_pos[ 4 ],
	Float:a_to_pos[ 4 ],
	a_virt_world [ 2 ],
	a_interior [ 2 ],
}

new area_info [ 250 ] [ _areas ];

enum _g_areas
{
	veh_shop [ 5 ],
}
new g_areas [ _g_areas ] ;

new Float:veh_coordinate [ 5 ] [ 7 ] ={
	{ -1658.7123, 1212.8981, 20.9499, 285.2443, -1659.106201, 1210.380737, 19.987318},
	{ -1658.7123, 1212.8981, 20.9499, 285.2443, -1659.106201, 1210.380737, 19.987318},
	{ -1658.7123, 1212.8981, 20.9499, 285.2443, -1659.106201, 1210.380737, 19.987318},
	{ -1658.7123, 1212.8981, 20.9499, 285.2443, -1659.106201, 1210.380737, 19.987318},
	{ 2088.2720, -120.2499, -0.1040, 113.8978, 2076.6804, -128.4288, 3.8201 }
};

new Float:veh_exit_coordinate [ 5 ] [ 8 ] ={
	{1226.3132,-669.7966,1086.9224,181.9727,1992.2520,2057.4917,10.6165,147.6579},
	{613.6450,-1558.8771,16.1713,88.3979,612.1320,-1574.9015,15.9149,269.6240},
	{1222.0387,-1190.9100,2401.2813,210.7421,-2551.2705,554.1206,14.6144,24.0162},
	{ 2131.9768,-1151.3162,24.0627,7.9869,2123.9871,-1145.0504,24.5080,321.7838},
	{ 2126.2383, -56.4087, 9.9030, 178.1346, 2112.8604, -42.3320, -0.1040, 90.0000}
};

new cars_for_sale [ 33 ] [ 5 ] ={
	{411,602,400,461,446},
	{415,587,401,462,452},
	{429,579,404,463,453},
	{451,565,405,468,454},
	{568,562,478,471,472},
	{494,560,549,481,473},
	{506,500,412,509,484},
	{495,559,419,510,493},
	{502,533,421,521,595},
	{503,558,422,581,0},
	{506,555,439,0,0},
	{541,550,445,0,0},
	{603,535,458,0,0},
	{402,490,529,0,0},
	{0,480,466,0,0},
	{0,426,467,0,0},
	{0,0,474,0,0},
	{0,0,475,0,0},
	{0,0,479,0,0},
	{0,0,489,0,0},
	{0,0,491,0,0},
	{0,0,492,0,0},
	{0,0,518,0,0},
	{0,0,516,0,0},
	{0,0,517,0,0},
	{0,0,0,0,0},
	{0,0,0,0,0},
	{0,0,0,0,0},
	{0,0,0,0,0},
	{0,0,0,0,0},
	{0,0,0,0,0},
	{0,0,0,0,0},
	{0,0,0,0,0}
};


new car_color [ 42 ] ={
	0,1,2,3,4,5,6,
	128,129,130,134,135,136,142,
	159,152,155,156,160,167,163,
	168,171,173,237,234,235,192,
	199,202,205,211,214,212,217,
	249,246,243,240,239,233,253
};
new get_td_salon_color [ 42 ] ={
	0x000000FF,0xF5F5F5FF,0x2A77A1FF,0x840410FF,0x263739FF,0x86446EFF,0xD78E10FF,
	0x177517FF,0x210606FF,0x125478FF,0x25225AFF,0x2C89AAFF,0x8A4DBDFF,0x817A26FF,
	0x2C1E08FF,0x1E4C99FF,0x1E9999FF,0x999976FF,0x142407FF,0x993E4DFF,0x16616FFF,
	0x481A0EFF,0x746D99FF,0x3E190CFF,0x7B3E7EFF,0x16301BFF,0x163012FF,0x999590FF,
	0x3A3A0BFF,0x15371CFF,0x060820FF,0x241663FF,0x516013FF,0x692015FF,0x52888EFF,
	0x4E0E27FF,0x355D8EFF,0x1B8A2FFF,0x319490FF,0x732617FF,0x7E1A6CFF,0x7B7E7DFF

};