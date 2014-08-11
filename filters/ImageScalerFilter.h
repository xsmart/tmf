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

#ifndef IMAGESCALERFILTER_H_
#define IMAGESCALERFILTER_H_

#include <string>
#include "tools/VideoScaler.h"
#include "Filter.h"

class ImageScalerFilter : public Filter {

private:

	VideoScaler * videoScaler;

	InputPort<Frame> * inputPortFrame;
	OutputPort<Frame> * outputPortFrame;

public:

	ImageScalerFilter(string name) : Filter(name) {

		inputPortFrame = new InputPort<Frame>("imageScaler, input, Frame",
				this);

		outputPortFrame = new OutputPort<Frame>("imageScaler, output, Frame",
				this);

		inputPorts.push_back(inputPortFrame);
		outputPorts.push_back(outputPortFrame);

		videoScaler = 0;
	}

	FilterStatus init() {

		MessageError err;
		FilterStatus status = FILTER_SUCCESS;

		string width = getProp("width");
		string height = getProp("height");

		int dstWidth = std::stoi(width);
		int dstHeight = std::stoi(height);

		//string videoName = getProp("input_video");

		int srcWidth, srcHeight, srcFormatInt;
		//VideoReader media = VideoReader(videoName);
		//int srcwidth = media.getWidth();
		//int srcheight = media.getHeight();
		//AVPixelFormat srcformat = media.getPixFormat();

		err = inMsg->getPropInt("width", srcWidth);
		if (err == MSG_NOT_FOUND)
			return FILTER_WAIT_FOR_INPUT;

		err = inMsg->getPropInt("height", srcHeight);
		if (err == MSG_NOT_FOUND)
			return FILTER_WAIT_FOR_INPUT;

		err = inMsg->getPropInt("format", srcFormatInt);
		if (err == MSG_NOT_FOUND)
			return FILTER_WAIT_FOR_INPUT;

		AVPixelFormat srcFormat = static_cast<AVPixelFormat>(srcFormatInt);



		videoScaler = new VideoScaler(srcWidth, srcHeight, srcFormat, dstWidth, dstHeight, srcFormat);

		for (int i=0; i<outputPortFrame->getBuffer()->getSize(); i++) {
			Frame * frame = outputPortFrame->getBuffer()->getNode(i);
			frame->fill(dstWidth, dstHeight, srcFormat);
		}

		outMsg->setProp("width", width);
		outMsg->setProp("height", height);
		outMsg->setPropInt("format", srcFormatInt);

		return status;

	}

	FilterStatus process() {
		FilterStatus status = FILTER_SUCCESS;

		Frame * inFrame = inputPortFrame->read();

		Frame * outFrame = outputPortFrame->getBuffer()->getNextNode();

		videoScaler->scale(inFrame, outFrame);

		outputPortFrame->produce(outFrame);
		outputPortFrame->process();

		return status;
	}




	~ImageScalerFilter() {
		delete inputPortFrame;
		delete outputPortFrame;
	}

};


#endif /* IMAGESCALERFILTER_H_ */