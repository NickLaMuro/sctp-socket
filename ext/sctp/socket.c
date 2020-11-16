#include "ruby.h"
#include <string.h>
#include <errno.h>
#include <netinet/sctp.h>

VALUE mSCTP;
VALUE cSocket;

static VALUE rsctp_init(int argc, VALUE* argv, VALUE self){
  int sock_fd;
  VALUE v_domain, v_type;

  rb_scan_args(argc, argv, "02", &v_domain, &v_type);

  if(NIL_P(v_domain))
    v_domain = INT2NUM(AF_INET);
  
  if(NIL_P(v_type))
    v_type = INT2NUM(SOCK_SEQPACKET);

  sock_fd = socket(NUM2INT(v_domain), NUM2INT(v_type), IPPROTO_SCTP);

  if(sock_fd < 0)
    rb_raise(rb_eSystemCallError, "socket: %s", strerror(errno));

  rb_iv_set(self, "@domain", v_domain);
  rb_iv_set(self, "@type", v_type);
  rb_iv_set(self, "@sock_fd", INT2NUM(sock_fd));

  return self;
}

static VALUE rsctp_bindx(int argc, VALUE* argv, VALUE self){
  VALUE v_addresses, v_port, v_family;
  rb_scan_args(argc, argv, "12", &v_addresses, &v_port, &v_family);

  return self;
}

static VALUE rsctp_close(VALUE self){
  VALUE v_sock_fd = rb_iv_get(self, "@sock_fd");

  if(close(NUM2INT(v_sock_fd)))
    rb_raise(rb_eSystemCallError, "close: %s", strerror(errno));

  return self;
}

void Init_socket(){
  mSCTP   = rb_define_module("SCTP");
  cSocket = rb_define_class_under(mSCTP, "Socket", rb_cObject);

  rb_define_method(cSocket, "initialize", rsctp_init, -1);

  rb_define_method(cSocket, "bindx", rsctp_bindx, -1);
  rb_define_method(cSocket, "close", rsctp_close, 0);

  rb_define_attr(cSocket, "domain", 1, 1);
  rb_define_attr(cSocket, "type", 1, 1);
  rb_define_attr(cSocket, "sock_fd", 1, 1);
}
