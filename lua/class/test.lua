require('class')
-- ref: http://lua-users.org/wiki/SimpleLuaClasses

A = class()
function A:init(x)
    self.x = x
end

function A:test()
  print(self.x)
end

t = A(5)

t:test()

B = class(A)

function B:init(x,y)
  A.init(self,x)
  self.y = y
end

function B:pr(self)
    print(self.y)
end
