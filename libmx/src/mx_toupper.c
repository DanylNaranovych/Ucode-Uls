int mx_toupper(int c) {
     if (c >= 'a' && c <= 'z')
        return c + 'A' - 'a';
    else
        return c;
}
