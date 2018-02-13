test_a = %q{10 3
		2 6
		6 8
		3 4}
# 5

test_b = %q{10 10
		1 6
		2 5
		1 7
		1 7
		2 7
		2 8
		1 4
		2 2
		1 10
		2 10}
# 1

test_c = %q{100 5
		1 100
		1 100
		1 100
		1 100
		1 100}
# 100

test_d = %q{100 20
		14 14
		53 100
		60 64
		78 82
		43 45
		6 7
		44 45
		83 98
		55 58
		34 34
		21 37
		17 73
		90 98
		55 62
		69 76
		25 73
		11 89
		92 94
		49 53
		17 17}
# 83

require 'set'

lines = test_a.split("\n")

$data = []
lines.each do |line|
    $data.push(line.strip.split.map {|x| x.to_i })
end

Paiza_submission = false
def read_line()
    if Paiza_submission
        return gets
    else
        return $data.shift
    end
end

print read_line

book = SortedSet.new [[3, 5], [1, 2], [6, 7]]


def process_user_input(book, input)

end