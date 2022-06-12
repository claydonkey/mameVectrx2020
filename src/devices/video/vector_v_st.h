// license:BSD-3-Clause
// copyright-holders:Mario Montminy, Anthony Campbell
#ifndef VECTOR_V_ST_H
#define VECTOR_V_ST_H
#pragma once

#include "osdcore.h"
#include "screen.h"
#include "divector.h"
 

struct serial_segment_t
{
	struct serial_segment_t* next;
	int intensity;
	int x0;
	int y0;
	int x1;
	int y1;

	serial_segment_t(
		int x0,
		int y0,
		int x1,
		int y1,
		int intensity) : next(NULL),
		intensity(intensity),
		x0(x0),
		y0(y0),
		x1(x1),
		y1(y1)
	{
	}
};

class vector_device_v_st_options
{
public:
	friend class vector_device_v_st;
	static float s_vector_scale;
	static float s_vector_scale_x;
	static float s_vector_scale_y;
	static float s_vector_offset_x;
	static float s_vector_offset_y;
	static char* s_vector_port;

	static int s_vector_rotate;
	static int s_vector_bright;

protected:
	static void init(emu_options& options);
};

class vector_device_v_st : public vector_device_t
{
public:
	vector_device_v_st(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock = 0);
	virtual void add_line(float xf0, float yf0, float xf1, float yf1, int intensity) override;
	virtual void add_point(int x, int y, rgb_t color, int intensity) override;
    virtual uint32_t screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect) override;

private:
	osd_file::ptr m_serial; 
	size_t m_serial_offset;
	int m_serial_drop_frame;
	int m_serial_sort;
	unsigned m_vector_transit[3];
	serial_segment_t* m_serial_segments;
	serial_segment_t* m_serial_segments_tail;
    std::unique_ptr<unsigned char[]> m_serial_buf;
	std::error_condition serial_write(uint8_t* buf, int size);
	int serial_read(uint8_t* buf, int size);
	int serial_send();
	void serial_draw_point(unsigned x, unsigned y, int intensity);
	void serial_reset();
protected:
    virtual void device_start() override;
    virtual void device_reset() override;
    virtual void device_stop() override;
};


// device type definition
DECLARE_DEVICE_TYPE(VECTOR_V_ST, vector_device_v_st)
#endif //VECTOR_V_ST_H
