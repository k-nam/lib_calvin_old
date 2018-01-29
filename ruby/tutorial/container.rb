module ContainerTest
    def ContainerTest.test_01()
        empty1 = []
        empty2 = Array.new
        words1 = ['Calvin', 'Tom']
        words2 = %w{ Calvin Tom} # returns array (automatic split)
        puts words1, words2
    end

    def ContainerTest.test_02()
        apple = 'apple'
        dictionary = { apple: 'リンゴ', "melon" => 'スイカ'  }
        data = Hash.new(0)      #default value
        data['height'] = 7
        dictionary[2] = 5
        puts dictionary[:apple]
        puts dictionary['pear']
        puts data['width']
    end
end