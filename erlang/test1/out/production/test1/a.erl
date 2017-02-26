%%%-------------------------------------------------------------------
%%% @author Calvin
%%% @copyright (C) 2015, <COMPANY>
%%% @doc
%%%
%%% @end
%%%-------------------------------------------------------------------
-module(a).

%% API
-export([qsort/1]).



qsort([])->[];
qsort([Pivot|S])->qsort([L||L<-S, L < Pivot])++[Pivot]++ qsort([R||R<-S, R >= Pivot]).



