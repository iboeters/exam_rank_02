#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

typedef struct      s_struct
{
    int     ret;
    int     i;
    int     pre;
    int     width;
}                   t_struct;

int     ft_isdigit(char c)
{
    if (c >= '0' && c <= '9')
        return (1);
    return (0);
}

int     ft_atoi(const char *str)
{
    int i;
    int sign;
    unsigned long int r;
    unsigned long int max_int;

    i = 0;
    sign = 1;
    r = 0;
    max_int = 922337203685477580;
    while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
        i++;
    if (str[i] == '-')
        sign = -1;
    if (str[i] == '-' || str[i] == '+')
        i++;
    while (str[i] >= '0' && str[i] <= '9')
    {
        if (sign == -1 && ((r == max_int && str[i+1] > '8') || r > max_int))
            return (0);
        else if (sign == 1 && ((r == max_int && str[i+1] > '7') || r > max_int))
            return (-1);
        r = r * 10 + str[i] - '0';
        i++;
    }
    return (r * sign);
}

void    ptf_putchar(char c, t_struct *spec)
{
    write(1, &c, 1);
    spec->ret++;
}

void     ptf_putnbr(int number, t_struct *spec)
{
    if (number < 0)
    {
        if (number == -2147483648)
        {
            ptf_putchar('2', spec);
            number = 147483648;
        }
        else
            number = -number;
    }
    if (number / 10 > 0)
        ptf_putnbr(number / 10, spec);
    ptf_putchar(number % 10 + '0', spec);
}

void    clean_struct(t_struct *spec)
{
    spec->width = 0;
    spec->pre = -1;
}

void      padding(int pad, t_struct *spec)
{
    while (pad > 0)
    {
        ptf_putchar(' ', spec);
        pad--;
    }
}

void      print_precision(int length, t_struct *spec)
{
    int l;

    l = length;
    while (spec->pre > l)
    {
        ptf_putchar('0', spec);
        l++;
    }
}

int      pad_amount(int number, int length, t_struct *spec)
{
    int pad;

    pad = 0;
    if (spec->width > 0)
    {
        if (spec->pre > length)
            pad = spec->width - spec->pre;
        else
            pad = spec->width - length;
    }
    return (pad);
}

void    print_hex(int number, char *c, t_struct *spec)
{
    while (number / 16 > 0)
        print_hex(number / 16, c, spec);
    ptf_putchar(c[number % 16], spec);
}

void     handle_hex(va_list *args, t_struct *spec)
{
    unsigned long int number;
    unsigned long int tmp_number;
    int pad;
    int length;
    char   *c;

    c = "0123456789abcdef";
    length = 0;
    number = va_arg(*args, unsigned long int);
    tmp_number = number;
    while (tmp_number != 0)
    {
        tmp_number = tmp_number / 16;
        length++;
    }
    if (number == 0)
        length = 1;
    if (number == 0 && spec->pre == 0)
        length = 0;
    pad = pad_amount(number, length, spec);
    padding(pad, spec);
    print_precision(length, spec);
    if (!(number == 0 && spec->pre == 0))
        print_hex(number, c, spec);
}

void      handle_int(va_list *args, t_struct *spec)
{
    int pad;
    int length;
    int tmp_number;
    int number;

    length = 0;
    number = va_arg(*args, int);
    tmp_number = number;
    while (tmp_number != 0)
    {
        tmp_number = tmp_number / 10;
        length++;
    }
    if (number == 0)
        length = 1;
    if (number == 0 && spec->pre == 0)
        length = 0;
    pad = pad_amount(number, length, spec);
    if (number < 0)
        pad--;
    padding(pad, spec);
    if (number < 0)
        ptf_putchar('-', spec);
    print_precision(length, spec);
    if (!(number == 0 && spec->pre == 0))
        ptf_putnbr(number, spec);
}

int   str_length(char **str)
{
    int length;

    length = 0;
    if (str == NULL)
    {
        length = 6;
        *str = "(null)";
        return (length);
    }
    else
    {
        while (str[length])
            length++;
    }
    return (length);
}

void        handle_str(va_list *args, t_struct *spec)
{
    int pad;
    int length;
    char   *str;
    int i;

    i = 0;
    str = va_arg(*args, void *);
    length = str_length(&str);
    if (spec->pre > length)
        pad = spec->width - spec->pre;
    else
        pad = spec->width - length;
    if (spec->pre == -1)
        spec->pre = length;
    padding(pad, spec);
    while (str[i] != '\0' && spec->pre > 0)
    {
        ptf_putchar(str[i], spec);
        spec->pre--;
        i++;
    }
}

int     find_specifiers(const char *s, va_list *args, t_struct *spec)
{
    if (s[spec->i] == 'd')
        handle_int(args, spec);
    else if (s[spec->i] == 'x')
        handle_hex(args, spec);
    else if (s[spec->i] == 's')
        handle_str(args, spec);
    else
        return (0);
    return (1);
}

int     ft_printf(const char *s, ...)
{
    va_list     args;
    t_struct    spec;

    spec.ret = 0;
    spec.i = 0;
    va_start(args, s);
    while (s[spec.i])
    {
        if (s[spec.i] != '%')
        {
            ptf_putchar(s[spec.i], &spec);
            spec.i++;
        }
        else
        {
            clean_struct(&spec);
            spec.i++;
            if (ft_isdigit(s[spec.i]))
                spec.width = ft_atoi(&s[spec.i]);
            while(ft_isdigit(s[spec.i]))
                spec.i++;
            if (s[spec.i] == '.')
                spec.i++;
            if (ft_isdigit(s[spec.i]))
                spec.pre = ft_atoi(&s[spec.i]);
            else if (s[spec.i - 1] == '.')
                spec.pre = 0;
            while (ft_isdigit(s[spec.i]))
                spec.i++;
            if (find_specifiers(s, &args, &spec))
                spec.i++;
        }
    }
    va_end(args);
    return (spec.ret);
}

int     main(void)
{
    int i;
    int ret;
    int ret2;

    i = -129;
    ft_printf("%10.d\n", 134);
    printf("%10.d\n", 134);
    printf("%10.9x\n", 10);
    ft_printf("%10.9x\n", 10);
    printf("%10.9s\n", "10");
    printf("%10.9s\n", "10");
    ft_printf("%10.9s\n", NULL);
    printf("%10.9s\n", NULL);
}