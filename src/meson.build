if get_option('gen_protobuf')
  protoc = find_program('protoc', required: true)
  run_command([protoc, '--c_out=./protobuf', 'netpoker.proto'], check: true)
endif

lib_src = files('lib.c')

net_shared_src = files(['game.c', 'net.c', 'netpoker.c', 'protobuf/netpoker.pb-c.c'])
