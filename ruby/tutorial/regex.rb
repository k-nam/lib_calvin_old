module RegexTest

    def RegexTest.test_01()
        def RegexTest.check_with_regex(input, regex)
            result = input =~ regex
            print input
            if result
                print " matched (position: #{result})"
            else
                print ' didnt match '
            end
            puts regex
        end
        check_with_regex('Calvin', /C.+n/)
        check_with_regex('Tom', /C.+n/)
    end

    def RegexTest.test_02()
        text = 'A000A00A'
        puts text.sub(/0+/, '')
        puts text.gsub(/0+/, '')
    end
end