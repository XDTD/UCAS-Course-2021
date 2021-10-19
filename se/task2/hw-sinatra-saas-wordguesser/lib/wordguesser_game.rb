class WordGuesserGame

  # add the necessary class methods, attributes, etc. here
  # to make the tests in spec/wordguesser_game_spec.rb pass.

  # Get a word from remote "random word" service
  guess_list = ''
  
  def initialize(word)
    @word = word
    @guess_list = ''
    @wrong_guess_list = ''
    @displayed = ''
    for i in 0...word.length
      @displayed += '-'
    end
    @valid = false
    @count = 0
  end

  # You can test it by installing irb via $ gem install irb
  # and then running $ irb -I. -r app.rb
  # And then in the irb: irb(main):001:0> WordGuesserGame.get_random_word
  #  => "cooking"   <-- some random word
  def self.get_random_word
    require 'uri'
    require 'net/http'
    uri = URI('http://randomword.saasbook.info/RandomWord')
    Net::HTTP.new('randomword.saasbook.info').start { |http|
      return http.post(uri, "").body
    }
  end

  def word
    return @word
  end

  def guesses
    return @guess_list
  end

  def wrong_guesses
    return @wrong_guess_list
  end

  # 给提示
  def word_with_guesses
    return @displayed
  end

  def check_win_or_lose
    if @displayed == @word 
      return :win
    elsif @count >= 7 
      return :lose
    else
      return :play 
    end 
  end

  def guess(word)
    # 局部变量，这次猜的返回值
    valid = false
    # 空就报错
    if word == '' or word == nil 
      raise ArgumentError
    end

    # 转小写
    word = word.downcase

    if !(('a'..'z') === word)
      raise ArgumentError
    end

    # 添加猜过的和猜错过的
    if @word.include? word  
      if !@guess_list.include? word
        @guess_list += word
        valid = true
        for i in 0...@word.length
          if @word[i] == word 
            @displayed[i] = word
          end
        end
      end

    elsif  !@word.include? word
      @count += 1  # 每错一次加一
      if !@wrong_guess_list.include? word
        @wrong_guess_list = @wrong_guess_list + word
        valid = true  # 不懂为什么这里是true
      end
    end
    
    return valid
  end
  
end
