/*
 *
 *  Tiny Multimedia Framework
 *  Copyright (C) 2014 Arash Shafiei
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef VIDEOFRAME_H_
#define VIDEOFRAME_H_

#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#ifdef __cplusplus
}
#endif

using namespace std;

class VideoFrameWriter {

private:
	string path;
	struct SwsContext *sws_ctx;
	AVFrame * pFrameRGB;
	uint8_t *buffer;
	int width;
	int height;
	AVPixelFormat format;
	int frameNumber;

public:

	VideoFrameWriter(int width, int height, AVPixelFormat format) {

		int numBytes;
		buffer = 0;
		path = "./";
		this->width = width;
		this->height = height;
		this->format = format;
		frameNumber = 0;

		// Allocate an AVFrame structure
		pFrameRGB = avcodec_alloc_frame();

		if (pFrameRGB == NULL) {
			std::cerr << "Cannot allocate frame." << endl;

		}

		// Determine required buffer size and allocate buffer
		numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, width, height);
		buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));

		sws_ctx = sws_getContext(width, height, format, width, height,
				AV_PIX_FMT_RGB24,
				SWS_BILINEAR,
				NULL,
				NULL,
				NULL);

		// Assign appropriate parts of buffer to image planes in pFrameRGB
		// Note that pFrameRGB is an AVFrame, but AVFrame is a superset
		// of AVPicture
		avpicture_fill((AVPicture *) pFrameRGB, buffer, AV_PIX_FMT_RGB24, width,
				height);

	}

	void writeImage(AVFrame * pFrame) {

		sws_scale(sws_ctx, (uint8_t const * const *) pFrame->data,
				pFrame->linesize, 0, height, pFrameRGB->data,
				pFrameRGB->linesize);

		FILE *pFile;
		char szFilename[32];
		int y;

		// Open file
		sprintf(szFilename, "%s/frame%d.ppm", path.c_str(), frameNumber++);
		pFile = fopen(szFilename, "wb");
		if (pFile == NULL)
			return;

		// Write header
		fprintf(pFile, "P6\n%d %d\n255\n", width, height);

		// Write pixel data
		for (y = 0; y < height; y++)
			fwrite(pFrameRGB->data[0] + y * pFrameRGB->linesize[0], 1,
					width * 3, pFile);

		// Close file
		fclose(pFile);

	}

	void setPath(string path) {
		this->path = path;
	}

	~VideoFrameWriter() {
		// Free the RGB image
		av_free(buffer);
		av_free(pFrameRGB);
	}

};

#endif /* VIDEOFRAME_H_ */
