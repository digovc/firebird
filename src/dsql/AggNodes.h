/*
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Adriano dos Santos Fernandes
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2010 Adriano dos Santos Fernandes <adrianosf@gmail.com>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef DSQL_AGG_NODES_H
#define DSQL_AGG_NODES_H

#include "../jrd/common.h"
#include "../jrd/blr.h"
#include "../dsql/Nodes.h"

namespace Jrd {


class AvgAggNode : public AggNode
{
public:
	explicit AvgAggNode(MemoryPool& pool, bool aDistinct, bool aDialect1, dsql_nod* aArg = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void make(dsc* desc, dsql_nod* nullReplacement);
	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ExprNode* copy(thread_db* tdbb, NodeCopier& copier) const;
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);

	virtual void aggInit(thread_db* tdbb, jrd_req* request) const;
	virtual void aggPass(thread_db* tdbb, jrd_req* request, dsc* desc) const;
	virtual dsc* aggExecute(thread_db* tdbb, jrd_req* request) const;

protected:
	virtual AggNode* dsqlCopy() const;

private:
	USHORT tempImpure;
};

class ListAggNode : public AggNode
{
public:
	explicit ListAggNode(MemoryPool& pool, bool aDistinct, dsql_nod* aArg = NULL,
		dsql_nod* aDelimiter = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void make(dsc* desc, dsql_nod* nullReplacement);
	virtual bool setParameterType(DsqlCompilerScratch* dsqlScratch,
		dsql_nod* node, bool forceVarChar) const;
	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ExprNode* copy(thread_db* tdbb, NodeCopier& copier) const;

	virtual void checkOrderedWindowCapable() const
	{
		Firebird::status_exception::raise(
			Firebird::Arg::Gds(isc_wish_list) <<
			Firebird::Arg::Gds(isc_random) <<
				"LIST is not supported in ordered windows");
	}

	virtual void aggInit(thread_db* tdbb, jrd_req* request) const;
	virtual void aggPass(thread_db* tdbb, jrd_req* request, dsc* desc) const;
	virtual dsc* aggExecute(thread_db* tdbb, jrd_req* request) const;

protected:
	virtual AggNode* dsqlCopy() const;

private:
	dsql_nod* dsqlDelimiter;
	jrd_nod* delimiter;
};

class CountAggNode : public AggNode
{
public:
	explicit CountAggNode(MemoryPool& pool, bool aDistinct, dsql_nod* aArg = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void make(dsc* desc, dsql_nod* nullReplacement);
	virtual void genBlr();
	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ExprNode* copy(thread_db* tdbb, NodeCopier& copier) const;

	virtual void aggInit(thread_db* tdbb, jrd_req* request) const;
	virtual void aggPass(thread_db* tdbb, jrd_req* request, dsc* desc) const;
	virtual dsc* aggExecute(thread_db* tdbb, jrd_req* request) const;

protected:
	virtual AggNode* dsqlCopy() const;
};

class SumAggNode : public AggNode
{
public:
	explicit SumAggNode(MemoryPool& pool, bool aDistinct, bool aDialect1, dsql_nod* aArg = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void make(dsc* desc, dsql_nod* nullReplacement);
	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ExprNode* copy(thread_db* tdbb, NodeCopier& copier) const;

	virtual void aggInit(thread_db* tdbb, jrd_req* request) const;
	virtual void aggPass(thread_db* tdbb, jrd_req* request, dsc* desc) const;
	virtual dsc* aggExecute(thread_db* tdbb, jrd_req* request) const;

protected:
	virtual AggNode* dsqlCopy() const;
};

class MaxMinAggNode : public AggNode
{
public:
	enum MaxMinType
	{
		TYPE_MAX,
		TYPE_MIN
	};

	explicit MaxMinAggNode(MemoryPool& pool, MaxMinType aType, dsql_nod* aArg = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void make(dsc* desc, dsql_nod* nullReplacement);
	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ExprNode* copy(thread_db* tdbb, NodeCopier& copier) const;

	virtual void aggInit(thread_db* tdbb, jrd_req* request) const;
	virtual void aggPass(thread_db* tdbb, jrd_req* request, dsc* desc) const;
	virtual dsc* aggExecute(thread_db* tdbb, jrd_req* request) const;

protected:
	virtual AggNode* dsqlCopy() const;

public:
	const MaxMinType type;
};


} // namespace

#endif // DSQL_AGG_NODES_H
