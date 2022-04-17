/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <libyul/AST.h>
#include <libyul/Dialect.h>
#include <libyul/Exceptions.h>

// because of instruction
#include <libyul/backends/evm/EVMDialect.h>

#include <libsmtutil/SolverInterface.h>

#include <memory>

namespace solidity::smtutil
{

class SolverInterface;
class Expression;
struct Sort;

}

namespace solidity::yul
{

/**
 * Base class for SMT based optimization steps.
 *
 * Works best when used with AST in SSA form.
 */
class SMTSolver
{
protected:
	SMTSolver(Dialect const& _dialect);

	/// Helper function that encodes VariableDeclaration
	void encodeVariableDeclaration(VariableDeclaration const& _varDecl);
	void encodeVariableAssignment(Assignment const& _assignment);
	void encodeVariableUpdate(YulString const& _name, Expression const& _value);
	void encodeVariableUpdateUnknown(YulString const& _name);

	/// The encoding for a builtin. The type of encoding determines what we are
	/// solving for.
	virtual smtutil::Expression encodeEVMBuiltin(
		evmasm::Instruction _instruction,
		std::vector<Expression> const& _arguments
	) = 0;

	smtutil::Expression encodeExpression(Expression const& _expression);

	static smtutil::Expression int2bv(smtutil::Expression _arg);
	static smtutil::Expression bv2int(smtutil::Expression _arg);

	std::string variableNameAtIndex(YulString const& _name, size_t _index) const;
	smtutil::Expression variableExpressionAtIndex(YulString const& _name, size_t _index) const;
	smtutil::Expression currentVariableExpression(YulString const& _name) const;

	smtutil::Expression newVariable();
	smtutil::Expression newBooleanVariable();
	virtual smtutil::Expression newRestrictedVariable(bigint _maxValue = (bigint(1) << 256) - 1);
	std::string uniqueName();

	virtual std::shared_ptr<smtutil::Sort> defaultSort() const;
	static smtutil::Expression booleanValue(smtutil::Expression _value);
	static smtutil::Expression constantValue(bigint _value);
	static smtutil::Expression literalValue(Literal const& _literal);
	/// Returns _value interpreted as a two's complement number and adds 2**256
	static smtutil::Expression twosComplementToUpscaledUnsigned(smtutil::Expression _value);
	static smtutil::Expression signedToTwosComplement(smtutil::Expression _value);
	smtutil::Expression wrap(smtutil::Expression _value);

	std::unique_ptr<smtutil::SolverInterface> m_solver;
	std::map<YulString, size_t> m_variableSequenceCounter;
	std::optional<smtutil::Expression> m_pathCondition;

	Dialect const& m_dialect;

private:
	size_t m_varCounter = 0;
};

}
