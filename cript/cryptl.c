#define _XOPEN_SOURCE
#include <unistd.h>
# include <crypt.h>

/*
 * cryptl() function forward to the crypt()
 * crypt()  is  the password encryption function
 *  If salt is "$1$" then it uses MD5-based algorithm
 */
char *cryptl(const char *key, const char *salt)
{
  return crypt(key, salt);
}