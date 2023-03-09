#include "hashids_ext.h"
#include <assert.h>
#include <string.h>
#include <math.h>

VALUE rb_mHashidsExt;
VALUE rb_cHashidsExtHashids;

void consistent_shuffle_buf(char buf[], const char* salt);

void
Init_hashids_ext(void)
{
    rb_mHashidsExt = rb_define_module("HashidsExt");
    VALUE rb_cHashids = rb_const_get(rb_cObject, rb_intern("Hashids"));
    rb_cHashidsExtHashids = rb_define_class_under(rb_mHashidsExt, "Hashids", rb_cHashids);

    // replace protected method
    rb_define_protected_method(rb_cHashidsExtHashids, "consistent_shuffle", consistent_shuffle, -1);
}

VALUE
consistent_shuffle(int argc, VALUE *argv, VALUE self)
{
    VALUE rb_alphabet = argv[0];
    VALUE rb_salt = argv[1];

    char* c_alphabet = StringValueCStr(rb_alphabet);
    int len = strlen(c_alphabet);

    char buf[(len) + 1];
    strncpy(buf, c_alphabet, len);
    buf[len] = '\0';

    consistent_shuffle_buf(buf, StringValueCStr(rb_salt));
    buf[len] = '\0';

    return rb_str_new_cstr(buf);
}

// inner function

void
consistent_shuffle_buf(char buf[], const char* salt)
{
    // return alphabet if salt.nil? || salt.empty?
    if (salt == NULL || strlen(salt) == 0) {
        return;
    }

    // chars = alphabet.each_char.to_a
    char* chars = buf;
    // salt_ords = salt.codepoints.to_a
    int len = strlen(salt);

    char salt_ords[(len) + 1];
    strncpy(salt_ords, salt, len);
    salt_ords[len] = '\0';

    // salt_length = salt_ords.length
    int salt_length = strlen(salt_ords);
    // idx = ord_total = 0
    int idx = 0;
    int ord_total = 0;

    // (alphabet.length-1).downto(1) do |i|
    for (int i = strlen(buf) - 1; i > 0; i--) {
        // ord_total += n = salt_ords[idx]
        int n = (int)salt_ords[idx];
        ord_total += n;

        // j = (n + idx + ord_total) % i
        int j = (n + idx + ord_total) % i;

        // chars[i], chars[j] = chars[j], chars[i]
        char ci = chars[i];
        char cj = chars[j];
        chars[i] = cj;
        chars[j] = ci;

        // idx = (idx + 1) % salt_length
        idx = (idx + 1) % salt_length;
    // end
    }
}
