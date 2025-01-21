use strict;
use warnings FATAL => 'all';

use PostgreSQL::Test::Cluster;
use PostgreSQL::Test::Utils;
use Test::More;


my $node = PostgreSQL::Test::Cluster->new('main');

$node->init;
$node->append_conf(
	'postgresql.conf',
	qq{work_mem = '8MB'});
$node->start;

# setup
$node->safe_psql("postgres",
		"CREATE EXTENSION hacking;\n");

# test read mode
my $result =
  $node->safe_psql("postgres", "SELECT hacking_function();");
like($result, qr/^$/, '');

# restart
$node->restart;

$node->wait_for_log(
	"database system is ready to accept connections"
);

$node->stop;

# control file should indicate normal shut down
command_like(
	[ 'pg_controldata', $node->data_dir() ],
	qr/Database cluster state:\s*shut down/,
	'cluster shut down normally');

done_testing();
