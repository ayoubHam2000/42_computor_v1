Main = \
	main.cpp

FILES = \
	$(Main)


CFLAGS = -Wall -Wextra -Werror
INCLUDE = -I ./includes

DEPFLAGS = -MMD -MF $(@:.o=.d)

NAME = computor

OBJ_DIR = _OUT/

OBJ = $(addprefix $(OBJ_DIR), $(FILES:.cpp=.o))

deps = $(OBJ:.o=.d)

all : $(NAME)

$(NAME) : $(OBJ)
	g++ $(OBJ) -o $(NAME) 

$(OBJ) : $(OBJ_DIR)%.o : %.cpp
	mkdir -p $(dir $@)
	g++ $(CFLAGS) $(INCLUDE) -c $< $(DEPFLAGS) -o $@

-include $(deps)

clean :
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -f $(NAME)

re : fclean all
