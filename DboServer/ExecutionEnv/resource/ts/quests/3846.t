CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 24;
	rq = 0;
	tid = 3846;
	title = 384602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 384609;
				ctype = 1;
				idx = 5631101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 17311201;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				m0widx = 15;
				m0fx = "-4344.35";
				m0fy = "0.000000";
				m0fz = "-5212.04";
				m0ttip = 384615;
				m0pat = 1;
				m1widx = 15;
				m1fx = "-5320.33";
				m1fy = "0.000000";
				m1fz = "-4264.14";
				m1ttip = 384615;
				m1pat = 1;
				cont = 384608;
				gtype = 2;
				area = 384601;
				goal = 384604;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 384605;
				stype = 256;
				taid = 1;
				title = 384602;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;1;0;";
			type = 0;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 384614;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 384601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5631101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 384607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 69;
			}
			CDboTSClickNPC
			{
				npcidx = "5631101;";
			}
			CDboTSCheckClrQst
			{
				and = "3844;";
				flink = 0;
				flinknextqid = "3847;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 384608;
			gtype = 2;
			oklnk = 2;
			area = 384601;
			goal = 384604;
			sort = 384605;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 384602;
		}
	}
}

