#pragma once

class EmulatedSymbol {
public:
	virtual void execute() {}
	virtual ~EmulatedSymbol() = default;
};