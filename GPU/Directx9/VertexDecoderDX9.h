// Copyright (c) 2012- PPSSPP Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0 or later versions.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official git repository and contact information can be found at
// https://github.com/hrydgard/ppsspp and http://www.ppsspp.org/.

#pragma once

#include "GPU/GPUState.h"
#include "Globals.h"
#include "base/basictypes.h"
#include "Core/Reporting.h"

#include "GPU/Common/VertexDecoderCommon.h"

class VertexDecoderDX9;

typedef void (VertexDecoderDX9::*StepFunction)() const;

// Right now
//   - compiles into list of called functions
// Future TODO
//   - will compile into lighting fast specialized x86 and ARM
class VertexDecoderDX9
{
public:
	VertexDecoderDX9() : coloff(0), nrmoff(0), posoff(0) {}
	~VertexDecoderDX9() {}

	// prim is needed knowledge for a performance hack (PrescaleUV)
	void SetVertexType(u32 vtype);
	u32 VertexType() const { return fmt_; }

	const DecVtxFormat &GetDecVtxFmt() { return decFmt; }

	void DecodeVerts(u8 *decoded, const void *verts, int indexLowerBound, int indexUpperBound) const;

	// This could be easily generalized to inject any one component. Don't know another use for it though.
	u32 InjectUVs(u8 *decoded, const void *verts, float *customuv, int count) const;

	bool hasColor() const { return col != 0; }
	int VertexSize() const { return size; }  // PSP format size

	void Step_WeightsU8() const;
	void Step_WeightsU16() const;
	void Step_WeightsFloat() const;

	void Step_TcU8() const;
	void Step_TcU16() const;
	void Step_TcFloat() const;

	void Step_TcU8Prescale() const;
	void Step_TcU16Prescale() const;
	void Step_TcFloatPrescale() const;

	void Step_TcU16Double() const;
	void Step_TcU16Through() const;
	void Step_TcU16ThroughDouble() const;
	void Step_TcFloatThrough() const;

	// TODO: tcmorph

	void Step_Color4444() const;
	void Step_Color565() const;
	void Step_Color5551() const;
	void Step_Color8888() const;

	void Step_Color4444Morph() const;
	void Step_Color565Morph() const;
	void Step_Color5551Morph() const;
	void Step_Color8888Morph() const;

	void Step_NormalS8() const;
	void Step_NormalS16() const;
	void Step_NormalFloat() const;

	void Step_NormalS8Morph() const;
	void Step_NormalS16Morph() const;
	void Step_NormalFloatMorph() const;

	void Step_PosS8() const;
	void Step_PosS16() const;
	void Step_PosFloat() const;

	void Step_PosS8Morph() const;
	void Step_PosS16Morph() const;
	void Step_PosFloatMorph() const;

	void Step_PosS8Through() const;
	void Step_PosS16Through() const;
	void Step_PosFloatThrough() const;

	void ResetStats() {
		memset(stats_, 0, sizeof(stats_));
	}

	void IncrementStat(int stat, int amount) {
		stats_[stat] += amount;
	}

	// output must be big for safety.
	// Returns number of chars written.
	// Ugly for speed.
	int ToString(char *output) const;

	// Mutable decoder state
	mutable u8 *decoded_;
	mutable const u8 *ptr_;

	// "Immutable" state, set at startup

	// The decoding steps
	StepFunction steps_[5];
	int numSteps_;

	u32 fmt_;
	DecVtxFormat decFmt;

	bool throughmode;
	int biggest;
	int size;
	int onesize_;

	int weightoff;
	int tcoff;
	int coloff;
	int nrmoff;
	int posoff;

	int tc;
	int col;
	int nrm;
	int pos;
	int weighttype;
	int idx;
	int morphcount;
	int nweights;

	int stats_[NUM_VERTEX_DECODER_STATS];
};

