require('class')

MAX_TURNS = 11
MAX_SHOUTS = MAX_TURNS*2
SHOUT1 = 1
SHOUT2 = 2
PINS_MAX = 10

-- list positions
FIRST_SHOUT = 1
SEC_SHOUT   = 2
SPARE       = 3
STRIKE      = 4
VOID        = -1

TRUE = 1
FALSE = 0

Game = class()

--%
--# Brief: Constructor for the class Game.
--#
function Game:init()

	self.turn = 1
	self.shout = SHOUT1
	self.pontuation = {}
	self.strike = 0

	for i=1, MAX_TURNS do
		table.insert(self.pontuation, {VOID, VOID, FALSE, FALSE})
	end
end

--%
--# Brief: Process the score for a shout.
--#
function Game:roll(pins)

	if self.turn >= MAX_TURNS then
		-- if the last turn had a strike or a spare, extra ball may be thrown.
		if not(((self.pontuation[self.turn - 1][SPARE] == TRUE) or (self.pontuation[self.turn - 1][STRIKE] == TRUE)) and (self.pontuation[MAX_TURNS][SHOUT1] == VOID)) then
		-- end of the game.
			print("\n\rEND OF THE GAME!!\n")
			self:score()
			return
		end
	end

	if (pins > PINS_MAX) or ((self.pontuation[self.turn][SHOUT1] + pins) > PINS_MAX) or (pins < 0) then
		print("\t\rInvalid pins quantity!\n")
		return
	end

	-- second shout
	if (self.shout == SHOUT2) then
		self:_secondShout(pins)

	-- first shout
	else
		self:_firstShout(pins)
	end
end

--%
--# Brief: Calculate the second shout of a turn.
--#
function Game:_secondShout(pins)

	self.pontuation[self.turn][self.shout] = pins

	if (self.pontuation[self.turn][SHOUT1] + pins) == PINS_MAX then
		self.pontuation[self.turn][SPARE] = TRUE
	end

	self.shout = SHOUT1
	self.turn = self.turn + 1
	return
end

--%
--# Brief: Calculate the first shout of a turn.
--#
function Game:_firstShout(pins)

	self.pontuation[self.turn][self.shout] = pins
	-- strike
	if pins == PINS_MAX then
		self.pontuation[self.turn][STRIKE] = TRUE
		self.turn = self.turn + 1

	else
		self.shout = SHOUT2
	return
	end
end

--%
--# Brief: Process the final score for the game.
--#
function Game:score()

	score = 0

	for turn=1, MAX_TURNS-1 do
		print("[" .. turn .. "] - \n\tShout 1: " .. self.pontuation[turn][SHOUT1] .. "\n\tShout 2: " .. self.pontuation[turn][SHOUT2] .. "\n\tSpare: " .. self.pontuation[turn][SPARE] .. "\n\tStrike: "  .. self.pontuation[turn][STRIKE])
		score = score + self.pontuation[turn][SHOUT1] + ((self.pontuation[turn][SHOUT2] >= 0) and self.pontuation[turn][SHOUT2] or 0)

		if self.pontuation[turn][SPARE] == TRUE then
			score = score + self.pontuation[turn+1][SHOUT1]

		elseif self.pontuation[turn][STRIKE] == TRUE then
			score = score + self.pontuation[turn+1][SHOUT1] + self.pontuation[turn+1][SHOUT2]
		end
		print("\n\tScore: " .. score)
	end

	print("Total score is: " .. score)

end

t = Game()

for i=1, 15 do
	t:roll(4)
end
t:roll(6)
t:roll(4)
t:roll(4)

t:score()
--t:roll(5)
--t:score()
